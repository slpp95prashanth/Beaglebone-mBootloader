#include <lib/irq.h>
#include <stdio.h>

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

int do_irq(void)
{
    int irq;

//    disable_irq();

    irq = get_irq();

    printf("%s irq=%p\n", __func__, irq);

    asm_handlers(irq);

    enable_irq();

    return 0;
}

#endif /* IRQ */
