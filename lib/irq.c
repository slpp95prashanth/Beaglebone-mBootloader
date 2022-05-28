#include <lib/irq.h>
#include <timer/timer.h>
#include <gpio/gpio.h>
#include <stdio.h>

#ifdef EXCEPTION

#ifdef IRQ

void irq_init(void)
{
	cpu_enable_intr();
#ifdef GPIO
	gpio_irq_init();
#endif
#ifdef TIMER
	//timer_irq_init();
#endif
    return ;
}

int request_irq(int irq, int (*handler)(int, void *), void *data)
{
    return asm_request_irq(irq, handler, data);
}

int do_irq(void)
{
    int irq;

    disable_irq();

    irq = get_irq();
#ifdef DEBUG_IRQ
    printf("%s irq=%p\n", __func__, irq);
#endif
    asm_handlers(irq);

    enable_irq();

    return 0;
}

#endif /* IRQ */

#endif /* EXCEPTION */
