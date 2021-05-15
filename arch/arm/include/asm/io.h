#ifndef _ASM_IO_H
#define _ASM_IO_H

#define writel(v, a) (*(volatile unsigned int *)(a) = (v))
#define readl(a) (*(volatile unsigned int *)(a))
#define writeb(v, a) (*(volatile unsigned char *)(a) = (v))
#define readb(a) (*(volatile unsigned char *)(a))

#define rmw_set(data, addr) \
	writel(readl(addr) | (data), addr)

#define rmw_clear(data, addr) \
	writel(readl(addr) & (~(data)), (addr))

static void __read_and_write(unsigned short val, unsigned int bit, void *addr)
{
	int mask = 0xffffffff;

	int tmp = readl(addr);
	mask &= 1 << bit;
	tmp |= ((val << bit) & mask);
	writel(tmp, addr);
}

static void set_and_wait_to_clear(unsigned int bit, void *addr)
{
	writel(1 << bit, addr);

	while (readl(addr) & (1 << bit));
}


#endif /* _ASM_IO_H */
