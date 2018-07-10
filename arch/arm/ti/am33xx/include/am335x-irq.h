#ifndef _AM335X_IRQ

#define AM335X_INTC_BASE    0x48200000

#define AM335X_INTC_MIR0   (AM335X_INTC_BASE + 0x84)
#define AM335X_INTC_MIR1   (AM335X_INTC_BASE + 0xa4)
#define AM335X_INTC_MIR2   (AM335X_INTC_BASE + 0xc4)
#define AM335X_INTC_MIR3   (AM335X_INTC_BASE + 0xe4)

#define AM335X_GPIO0_INTR   96

#define MAX_IRQS 127

struct irq_handlers {
    int (*handler)(int, void *);
    void *data;
};

extern int asm_request_irq(int, int (*)(int, void *), void *);
extern int asm_request_free(int);

#endif /* _AM335X_IRQ */
