#include <asm/io.h>
#include <stdio.h>

void __read_and_write(bool val, bool bit, uint32_t addr)
{
	int mask = 0xffffffff;

	int tmp = readl(addr);
	mask &= 1 << bit;
	tmp |= ((val << bit) & mask);
	writel(tmp, addr);
}

void set_and_wait_to_clear(bool bit, uint32_t addr)
{
	writel(1 << bit, addr);

	while (readl(addr) & (1 << bit));
}


