#define writel(v, a) (*(volatile unsigned int *)(a) = (v))
#define readl(a) (*(volatile unsigned int *)a)
#define writeb(v, a) (*(volatile unsigned char *)(a) = (v))
#define readb(a) (*(volatile unsigned char *)a)

typedef unsigned int long ulong;

static void watchdog_disable(void)
{
        writel(0xAAAA, 0x44e35048);
        while (readl(0x44e35034) != 0x0)
                ;
        writel(0x5555, 0x44e35048);
        while (readl(0x44e35034) != 0x0)
                ;
}

#define CTRL_BASE                       0x44E10000

void uart_soft_reset(void)
{
#define UART_RESET              (0x1 << 1)
#define UART_CLK_RUNNING_MASK   0x1
#define UART_SMART_IDLE_EN      (0x1 << 0x3)
    unsigned int regVal;
    regVal = readl(0x44e09054);
    regVal |= UART_RESET;
    writel(regVal, 0x44e09054);
    while ((readl(0x44e09058) &
	UART_CLK_RUNNING_MASK) != UART_CLK_RUNNING_MASK);

    /* Disable smart idle */
    regVal = readl(0x44e09054);
    regVal |= UART_SMART_IDLE_EN;
    writel(regVal, 0x44e09054);
}

void uart_clock_enable(void)
{
    writel(2, 0x44e004b4);
}

void set_uart_mux_conf(void)
{
    writel(0x00000030, CTRL_BASE + 2416);
    writel(0, CTRL_BASE + 2420);
}

void uart_init(void)
{
#include "ns16550.h"
        struct ns16550 *com_port = (struct ns16550 *)0x44e09000;
        int baud_divisor = 26;

        writeb(CONFIG_SYS_NS16550_IER, &com_port->ier);
        writeb(0x7, &com_port->mdr1);   /* mode select reset TL16C750*/
        writeb(UART_LCR_BKSE | UART_LCRVAL, (ulong)&com_port->lcr);
        writeb(0, &com_port->dll);
        writeb(0, &com_port->dlm);
        writeb(UART_LCRVAL, &com_port->lcr);
        writeb(UART_MCRVAL, &com_port->mcr);
        writeb(UART_FCRVAL, &com_port->fcr);
        writeb(UART_LCR_BKSE | UART_LCRVAL, &com_port->lcr);
        writeb(baud_divisor & 0xff, &com_port->dll);
        writeb((baud_divisor >> 8) & 0xff, &com_port->dlm);
        writeb(UART_LCRVAL, &com_port->lcr);
        writeb(0, &com_port->mdr1);
	while(1) {
	    while ((readb(&com_port->lsr) & UART_LSR_THRE) == 0)
		; 
            writeb('1', &com_port->thr);
	    while ((readb(&com_port->lsr) & UART_LSR_THRE) == 0)
		; 
            writeb('2', &com_port->thr);
	}
}
void early_system_init(void)
{
        /*
         * The ROM will only have set up sufficient pinmux to allow for the
         * first 4KiB NOR to be read, we must finish doing what we know of
         * the NOR mux in this space in order to continue.
         */
    watchdog_disable();
    uart_clock_enable();
    set_uart_mux_conf();

    uart_soft_reset();
    uart_init();

//    do_check_uart();

    return;
}

void board_init_f(ulong dummy)
{
    early_system_init();
    return;
}
