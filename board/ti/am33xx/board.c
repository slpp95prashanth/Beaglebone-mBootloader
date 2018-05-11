#include<asm/io.h>
#include<serial/ns16550.h>
#include<wdt.h>

#include"clock.h"
#include"ctrl-module.h"

typedef unsigned int long ulong;

static void watchdog_disable(void)
{
        writel(0xAAAA, AM335X_WDT1_WSPR);
        while (readl(AM335X_WDT1_WWPS) != 0x0)
                ;
        writel(0x5555, AM335X_WDT1_WSPR);
        while (readl(AM335X_WDT1_WWPS) != 0x0)
                ;
}

#ifdef SERIAL_UART

void uart_soft_reset(void)
{
    unsigned int regVal;

    regVal = readl(AM335X_SERIAL_UART0_SYSCON);
    regVal |= UART_RESET;
    writel(regVal, AM335X_SERIAL_UART0_SYSCON);

    while ((readl(AM335X_SERIAL_UART0_SYSSTATS) & 
		UART_CLK_RUNNING_MASK) != UART_CLK_RUNNING_MASK);

    /* Disable smart idle */
    regVal = readl(AM335X_SERIAL_UART0_SYSCON);
    regVal |= UART_SMART_IDLE_EN;
    writel(regVal, AM335X_SERIAL_UART0_SYSCON);
}

void uart_clock_enable(void)
{
    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_UART0);
    while (readl(AM335X_CLK_WKUP_UART0) != PRCM_MOD_EN)
           ;
}

void set_uart_mux_conf(void)
{
    writel(MODE(0) | PULLUP_EN | RXACTIVE, AM335X_CTRL_CONF_UART0_RXD);
    writel(MODE(0) | PULLUDEN, AM335X_CTRL_CONF_UART0_TXD);
}

#endif /* SERIAL_UART */

void early_system_init(void)
{
        /*
         * The ROM will only have set up sufficient pinmux to allow for the
         * first 4KiB NOR to be read, we must finish doing what we know of
         * the NOR mux in this space in order to continue.
         */
    watchdog_disable();
#ifdef SERIAL_UART
    uart_clock_enable();
    set_uart_mux_conf();

    uart_soft_reset();
    uart_init();

#ifdef SERIAL_DEBUG_CONSOLE
    do_check_uart();
#endif

#endif /* SERIAL_UART */

#ifdef SHELL
    shell_start();
#endif
    return;
}

void board_init_f(ulong dummy)
{
    early_system_init();
    return;
}
