#include <serial/uart.h>
#include <serial/ns16550.h>
#include <stdio.h>

char getc(void)
{
	char ch;

	ch = NS16550_getc();

	return ch;
}

void putc(char ch)
{
	NS16550_putc(ch);
	return ;
}

#ifdef DEBUG_PRINTF

void tmp_putc1(int *dummy, char ch)
{
	if (ch == '\n') {
		putc('\r');
	}

	putc(ch);
	return ;
}

#endif /* DEBUG_PRINTF */

void gets(char *str)
{
	if (str == NULL) {
		return ;
	}

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
	if (str == NULL) {
		return ;
	}

	while(*str != '\0') {
		if (*str == '\n') {
			NS16550_putc('\r');
		}

		NS16550_putc(*str++);
	}

	return ;
}





 
