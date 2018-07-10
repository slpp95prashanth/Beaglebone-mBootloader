#include <asm/io.h>
#include <asm/types.h>
#include <lib/math.h>
#include <am335x-irq.h>
#include <stdio.h>

#ifdef IRQ

static struct irq_handlers handlers[MAX_IRQS + 1];

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

#ifdef IRQ_DEFAULT_HANDLER
    if (handler == NULL) {
	handlers[irq].handler = default_handler;
    }
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

#endif /* IRQ */
