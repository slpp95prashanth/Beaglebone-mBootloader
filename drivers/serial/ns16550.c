#include<serial/ns16550.h>
#include<asm/types.h>
#include<asm/io.h>
void do_check_uart(void)
{
    struct ns16550 *com_port = (struct ns16550 *)0x44e09000;
    while(1) {
            while ((readb(&com_port->lsr) & UART_LSR_THRE) == 0)
                ;
            writeb('1', &com_port->thr);
            while ((readb(&com_port->lsr) & UART_LSR_THRE) == 0)
                ;
            writeb('2', &com_port->thr);
        }
}
void uart_init(void)
{
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
}


 
