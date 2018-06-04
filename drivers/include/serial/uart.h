#ifndef _SERIAL_UART_H
#define _SERIAL_UART_H

#include<asm/types.h>

#define CONSOLE_UART_PORT 0
#define CONSOLE_UART_BAUDRATE 115200
#define ONE_STOP_BITS 1
#define NO_PARITY 4

struct uart {
    char *name;
    uint16_t port;
    uint16_t mode;
    uint32_t baudrate;
    void (*putc)(char);
    char (*getc)(void);
};

int uart_console_init(void);
int uart_dev_init(struct uart *);

#endif /* _SERIAL_UART_H */
