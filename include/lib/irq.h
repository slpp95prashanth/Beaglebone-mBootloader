#ifndef _LIB_IRQ

void irq_init(void);
int request_irq(int, int (*)(int, void *), void *);
int asm_request_irq(int, int (*)(int, void *), void *);
int asm_handlers(int);
void enable_irq(void);
void disable_irq(void);
int get_irq(void);

void gpio_irq_init(void);

void cpu_disable_intr(void);
void cpu_enable_intr(void);

void print_irq_enabled(void);
void print_irq_count(void);
#endif /* _LIB_IRQ */
