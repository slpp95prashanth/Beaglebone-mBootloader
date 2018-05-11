#include<serial/ns16550.h>
#include<asm/types.h>
#include<asm/io.h>

#ifdef SERIAL_UART

void NS16550_putc(char c)
{
        struct ns16550 *com_port = (struct ns16550 *)AM335X_SERIAL_UART0;
	
        while ((readb(&com_port->lsr) & UART_LSR_THRE) == 0)
                ;
        writeb(c, &com_port->thr);

	return ;
}

char NS16550_getc(void)
{
        struct ns16550 *com_port = (struct ns16550 *)AM335X_SERIAL_UART0;

        while ((readb(&com_port->lsr) & UART_LSR_DR) == 0);

        return readb(&com_port->rbr);
}
#ifdef SERIAL_DEBUG_CONSOLE

void do_check_uart(void)
{
    char ch = 'w', str[120];
    struct ns16550 *com_port = (struct ns16550 *)AM335X_SERIAL_UART0;

    int i;

    NS16550_putc('\r');
    NS16550_putc('\n');
    NS16550_putc('1');
    NS16550_putc('2');
    NS16550_putc('\r');
    NS16550_putc('\n');

    puts("\t\t\tGES\n");

    NS16550_putc('\r');
    NS16550_putc('\n');

    puts(str);

    NS16550_putc('\r');
    NS16550_putc('\n');

    memset(str, 'w', 21);

    puts(str);

    memset(str, 'w', 70);

    puts(str);

    NS16550_putc('\r');
    NS16550_putc('\n');

    for (i = 0 ; i < 70 ; i++) {
	putc(str[(i)]);
    }

    NS16550_putc('\r');
    NS16550_putc('\n');
}

#endif /* SERIAL_DEBUG_CONSOLE */

void uart_init(void)
{
        struct ns16550 *com_port = (struct ns16550 *)AM335X_SERIAL_UART0;
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

#endif /* SERIAL_UART */

