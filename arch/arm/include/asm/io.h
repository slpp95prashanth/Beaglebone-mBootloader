#ifndef _ASM_IO_H
#define _ASM_IO_H

#define writel(v, a) (*(volatile unsigned int *)(a) = (v))
#define readl(a) (*(volatile unsigned int *)(a))
#define writeb(v, a) (*(volatile unsigned char *)(a) = (v))
#define readb(a) (*(volatile unsigned char *)(a))


#endif /* _ASM_IO_H */
