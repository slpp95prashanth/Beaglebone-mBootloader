#include <asm/io.h>
#include <timer/timer.h>
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
extern void ddr_init(void);

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

void uart_clock_enable(void)
{
    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_UART0);
    while (readl(AM335X_CLK_WKUP_UART0) != PRCM_MOD_EN)
           ;
}

#endif /* SERIAL_UART */

#ifdef GPIO

void gpio_clock_enable(void)
{
    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_GPIO0);
    while (readl(AM335X_CLK_WKUP_GPIO0) != PRCM_MOD_EN)
           ;
    writel(PRCM_MOD_EN, AM335X_CM_PER_GPIO1_CLKCTRL);
    while (readl(AM335X_CM_PER_GPIO1_CLKCTRL) != PRCM_MOD_EN)
           ;
}

#endif /* GPIO */

#ifdef TIMER

void timer_clock_enable(void)
{
    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_TIMER0);
    while (readl(AM335X_CLK_WKUP_TIMER0) != PRCM_MOD_EN)
           ;

    return ;
}

#endif /* TIMER */

#ifdef DDR

void enable_emif_clocks(void)
{
        /* Enable the  EMIF_FW Functional clock */
        writel(PRCM_MOD_EN, AM335X_CM_PER_EMIF_FW_CLKCTRL);
        /* Enable EMIF0 Clock */
        writel(PRCM_MOD_EN, AM335X_CM_PER_EMIF_CLKCTRL);
        /* Poll if module is functional */
        while ((readl(AM335X_CM_PER_EMIF_CLKCTRL)) != PRCM_MOD_EN)
                ;
}

#endif /* DDR */

void dev_clk_enable(void)
{
#ifdef SERIAL_UART
    uart_clock_enable();
#endif
#ifdef GPIO
    gpio_clock_enable();
#endif
#ifdef TIMER
    timer_clock_enable();
#endif
#ifdef DDR
    enable_emif_clocks();
#endif
    return ;
}

void early_system_init(void)
{
        /*
         * The ROM will only have set up sufficient pinmux to allow for the
         * first 4KiB NOR to be read, we must finish doing what we know of
         * the NOR mux in this space in order to continue.
         */
//    cache_disable(1<<2);
    watchdog_disable();

    config_ctrl_module();

    dev_clk_enable();
#ifdef SERIAL_UART
    uart_console_init();

#ifdef DEBUG_PRINTF
extern void tmp_putc1(int *, char);
//void (*tmp_putc)(int *, char) = &tmp_putc1;
    init_printf(0, &tmp_putc1);
#endif /* DEBUG_PRINTF */

#endif /* SERIAL_UART */
#ifdef EXCEPTION
    asm_exception();

#ifdef IRQ
    irq_init();
#endif
#endif

#ifdef TIMER
    timer_init(0, (void *)(2));
#endif

#ifdef DDR
    ddr_init();
#endif
#ifdef SERIAL_UART

#ifdef SHELL
    shell_start();
#else
    while(1);
#endif

#endif /* SERIAL_UART */

    return;
}

void board_init_f(ulong dummy)
{
    early_system_init();
    return;
}
