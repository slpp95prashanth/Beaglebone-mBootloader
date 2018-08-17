#include <serial/uart.h>

#ifdef SERIAL_UART

int uart_console_init(void)
{
    struct uart uart;

    uart.baudrate = CONSOLE_UART_BAUDRATE;
    uart.mode = ONE_STOP_BITS | NO_PARITY;
    uart.port = CONSOLE_UART_PORT;

    return uart_dev_init(&uart);
}

#ifdef UART1

void uart_port_1(void)
{
    struct uart uart;

    uart.baudrate = CONSOLE_UART_BAUDRATE;
    uart.mode = ONE_STOP_BITS | NO_PARITY;
    uart.port = 1;

    uart_dev_init(&uart);

    return ;
}

#endif

#endif /* SERIAL_UART */
