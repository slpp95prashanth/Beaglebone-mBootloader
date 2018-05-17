#include<serial/uart.h>

int uart_console_init(void)
{
    struct uart uart;

    uart.baudrate = CONSOLE_UART_BAUDRATE;
    uart.mode = ONE_STOP_BITS | NO_PARITY;
    uart.port = CONSOLE_UART_PORT;

    return uart_dev_init(&uart);
}
 
