#include<serial/ns16550.h>

char getc(void)
{
    char ch;

    ch = NS16550_getc();
    NS16550_putc(ch);		/* illusion of typing char */

    return ch;
}

void putc(char ch)
{
    NS16550_putc(ch);
    return ;
}

void gets(char *str)
{
    do {
	*str = NS16550_getc();
        NS16550_putc(*str);
    } while (*str++ != '\r');

    *--str = '\0';

    NS16550_putc('\r');
    NS16550_putc('\n');

    return ;
}

void puts(char *str)
{
    while(*str != '\0') {

	if (*str == '\n')
	    NS16550_putc('\r');

	NS16550_putc(*str++);
    }

    return ;
}





 
