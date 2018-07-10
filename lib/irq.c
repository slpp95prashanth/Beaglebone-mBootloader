#include <lib/irq.h>

#ifdef IRQ

void irq_init(void)
{
    cpu_irq_init();
    gpio_irq_init();

    return ;
}

int request_irq(int irq, int (*handler)(int, void *), void *data)
{
    return asm_request_irq(irq, handler, data);
}

#endif /* IRQ */
