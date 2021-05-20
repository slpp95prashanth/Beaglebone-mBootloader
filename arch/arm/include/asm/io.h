#ifndef _ASM_IO_H
#define _ASM_IO_H

#include <asm/types.h>

#define writel(v, a) (*(volatile unsigned int *)(a) = (v))
#define readl(a) (*(volatile unsigned int *)(a))
#define writeb(v, a) (*(volatile unsigned char *)(a) = (v))
#define readb(a) (*(volatile unsigned char *)(a))

#define rmw_set(data, addr) \
	writel(readl(addr) | (data), addr)

#define rmw_clear(data, addr) \
	writel(readl(addr) & (~(data)), (addr))

void __read_and_write(bool val, bool bit, uint32_t addr);
void set_and_wait_to_clear(bool bit, uint32_t addr);

#endif /* _ASM_IO_H */
