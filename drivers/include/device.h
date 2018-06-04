#ifndef _DEV_H
#define _DEV_H

#include<serial/uart.h>

struct console {
    void (*putc)(char);
    char (*getc)(void);
    void *dev;
};

struct device {
    struct console *console;
    struct uart *uart;
};





#endif /* _DEV_H */


