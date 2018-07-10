#include <asm/io.h>
#include <serial/ns16550.h>
#include <serial/uart.h>
#include <wdt.h>
#include <am335x-irq.h>
#include "clock.h"
#include "ctrl-module.h"
#include <stdio.h>
#include <cmd.h>

extern void asm_exception(void);
extern void irq_init(void);

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

void uart_clock_enable(void)
{
    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_UART0);
    while (readl(AM335X_CLK_WKUP_UART0) != PRCM_MOD_EN)
           ;
}

void gpio_clock_enable(void)
{
    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_GPIO0);
    while (readl(AM335X_CLK_WKUP_GPIO0) != PRCM_MOD_EN)
           ;
    writel(PRCM_MOD_EN, AM335X_CM_PER_GPIO1_CLKCTRL);
    while (readl(AM335X_CM_PER_GPIO1_CLKCTRL) != PRCM_MOD_EN)
           ;
}

void dev_clk_enable(void)
{
    uart_clock_enable();
    gpio_clock_enable();
    return ;
}

void early_system_init(void)
{
        /*
         * The ROM will only have set up sufficient pinmux to allow for the
         * first 4KiB NOR to be read, we must finish doing what we know of
         * the NOR mux in this space in order to continue.
         */
    watchdog_disable();

    config_ctrl_module();

    dev_clk_enable();
#ifdef SERIAL_UART
    uart_console_init();

#endif /* SERIAL_UART */

#ifdef EXCEPTION
    asm_exception();
#endif

#ifdef IRQ
    irq_init();
#endif

#ifdef SERIAL_UART

#ifdef DEBUG_PRINTF
extern void tmp_putc1(int *, char);
//void (*tmp_putc)(int *, char) = &tmp_putc1;
    init_printf(0, &tmp_putc1);
#endif /* DEBUG_PRINTF */

#ifdef SHELL
    shell_start();
#endif

#endif /* SERIAL_UART */
    return;
}

void board_init_f(ulong dummy)
{
    early_system_init();
    return;
}
