#include<serial/ns16550.h>
#include<asm/types.h>
#include<asm/io.h>

void NS16550_putc(char c)
{
        struct ns16550 *com_port = (struct ns16550 *)0x44e09000;
	
        while ((readb(&com_port->lsr) & UART_LSR_THRE) == 0)
                ;
        writeb(c, &com_port->thr);

	return ;
}

char NS16550_getc(void)
{
        struct ns16550 *com_port = (struct ns16550 *)0x44e09000;

        while ((readb(&com_port->lsr) & UART_LSR_DR) == 0);

        return readb(&com_port->rbr);
}

void do_check_uart(void)
{
    char ch = 'w', *str = (char *)0x402f2fff;
    struct ns16550 *com_port = (struct ns16550 *)0x44e09000;

    NS16550_putc('\r');
    NS16550_putc('\n');
    NS16550_putc('1');
    NS16550_putc('2');
    NS16550_putc('\r');
    NS16550_putc('\n');

//    ch = NS16550_getc();
//    NS16550_putc(ch);

    while(1) {
	puts("\t\t\tGES\n");
	gets(str);
	puts(str);
        NS16550_putc('\r');
        NS16550_putc('\n');
    }
//	NS16550_putc('1');
//	NS16550_putc('2');
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


 
