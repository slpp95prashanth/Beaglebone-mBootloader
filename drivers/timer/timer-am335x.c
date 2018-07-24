#include <asm/io.h>
#include <lib/irq.h>
#include <stdio.h>
#include <gpio/gpio.h>
#include <am335x-timer.h>
#include <am335x-irq.h>
#include <am335x-gpio.h>
#include <timer/timer.h>

#ifdef TIMER

void timer_init(int irq, void *secs)
{
    int loader;

    /* ACK interrupt */
    writel(OVF_EN_FLAG, AM335X_DMTIMER0_IRQSTATUS);

#ifdef GPIO

    gpio_direction_out(USER_LED0);

    if (gpio_get(USER_LED0) == (1)) {
	gpio_clear(USER_LED0);
    } else {
	gpio_set(USER_LED0);
    }

#endif /* GPIO */

    loader = (unsigned int)0xffffffff - ((int)AM335X_DMTIMER0_CLK * (int)secs);

    writel(loader, AM335X_DMTIMER0_TLDR);
    writel(loader, AM335X_DMTIMER0_TCRR);

    writel(OVF_EN_FLAG, AM335X_DMTIMER0_IRQENABLE_SET);

    writel(START_TIMER | AUTO_RELOAD | TRIGGER_ON_OVERFLOW, AM335X_DMTIMER0_TLCR);

    return ;
}

#ifdef IRQ

void timer_irq_init(void)
{
    request_irq(TIMER0_INTR, (int (*)(int, void *))timer_init, (void *)2);
}

#endif

#endif  /* TIMER */
