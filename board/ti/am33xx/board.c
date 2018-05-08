#include<asm/io.h>

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

    do_check_uart();

    shell_start();

    return;
}

void board_init_f(ulong dummy)
{
    early_system_init();
    return;
}
