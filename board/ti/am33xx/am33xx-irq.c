#include <asm/io.h>
#include <asm/types.h>
#include <lib/math.h>
#include <am335x-irq.h>
#include <stdio.h>
#include <lib/irq.h>

#ifdef EXCEPTION

#ifdef IRQ

static struct irq_handlers handlers[MAX_IRQS + 1];

void disable_irq(void)
{
	cpu_disable_intr();
}

void enable_irq(void)
{
	writel(ENABLE_NEW_IRQ, AM335X_INTC_CONTROL);

	cpu_irq_init_pre_mode_spsr();

	__asm(" dsb");

	cpu_enable_intr();
}

int get_irq(void)
{
    return readl(AM335X_INTC_SIR_IRQ) & IRQ_MASK;
}

int asm_request_irq(int irq, int (*handler)(int, void *), void *data)
{
    uint8_t off, index;
    uint32_t *addr;

    uint32_t mir[] = {AM335X_INTC_MIR0, AM335X_INTC_MIR1, AM335X_INTC_MIR2, AM335X_INTC_MIR3};

    if (irq > 127) {
	return -1;
    }

    off = idiv_by_pow16(irq, 32);
    index = imod_by_pow16(irq, 32);

    addr = (uint32_t *)mir[off];

    writel(readl(addr) & ~(1 << index), addr);

#if defined(DEBUG_PRINTF) && defined(DEBUG_IRQ)
    printf("handlers %p ", irq);
    printf("handler %p ", handler);
    printf("data %p\n", data);
#endif

    handlers[irq].handler = handler;
    handlers[irq].data = data;

    return 0;
}

int asm_request_free(int irq)
{
    uint8_t off, index;
    uint32_t *addr;

    uint32_t mir[] = {AM335X_INTC_MIR0, AM335X_INTC_MIR1, AM335X_INTC_MIR2, AM335X_INTC_MIR3};

    if (irq > 127) {
	return -1;
    }

    off = idiv_by_pow16(irq, 32);
    index = imod_by_pow16(irq, 32);

    addr = (uint32_t *)mir[off];

    handlers[irq].handler = NULL;
    handlers[irq].data = NULL;

    writel(readl(addr) | (1 << index), addr);

    return 0;
}

int asm_handlers(int irq)
{ 
	if (handlers[irq].handler != (NULL)) {
		return handlers[irq].handler((irq), handlers[irq].data);
	}	

	return -1;
}

#endif /* IRQ */
#endif /* EXCEPTION */
