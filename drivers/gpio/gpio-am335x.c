#include <asm/io.h>
#include <asm/types.h>
#include <am335x-gpio.h>
#include <stdio.h>
#include <am335x-irq.h>
#include <gpio/gpio.h>

#ifdef GPIO

static uint32_t gpio_bank[] = {AM335x_GPIO0_BASE, AM335x_GPIO1_BASE};

#if defined(EXCEPTION) && defined(IRQ)

int _gpio0_irq_handler(int irq, void *data)
{
    char *addr = (char *)gpio_bank[0];

/* Interrupt ACK */
    while(readl(addr + AM335X_GPIO_IRQSTATUS_0)) {
	writel(readl(addr + AM335X_GPIO_IRQSTATUS_0), \
		(addr + AM335X_GPIO_IRQSTATUS_0));
	printf("%p\n", (readl(addr + AM335X_GPIO_IRQSTATUS_0)));
    }

    printf("%s irq=%p data=%p\n", __func__, irq, data);

    return 0;
}

int _gpio1_irq_handler(int irq, void *data)
{
    char *addr = (char *)gpio_bank[1];

/* Interrupt ACK */
    while(readl(addr + AM335X_GPIO_IRQSTATUS_0)) {
	writel(readl(addr + AM335X_GPIO_IRQSTATUS_0), \
		(addr + AM335X_GPIO_IRQSTATUS_0));
	printf("%p\n", (readl(addr + AM335X_GPIO_IRQSTATUS_0)));
    }

    printf("%s irq=%p data=%p\n", __func__, irq, data);

    return 0;
}

void gpio_irq_init(void)
{
    asm_request_irq(AM335X_GPIO0_INTR, _gpio0_irq_handler, gpio_bank);
    asm_request_irq(AM335X_GPIO1_INTR, _gpio1_irq_handler, gpio_bank);

    return ;
}

int _gpio_irq(int bank, int gpio, int mode)
{
    uint32_t addr, direction;

    addr = gpio_bank[bank];

    direction = readl(addr + AM335X_GPIO_OE) & (1 << gpio);

    if (!direction) {
	return -1;
    }
 
    rmw_set(1 << gpio, addr + AM335X_GPIO_IRQSTATUS_SET_0);

    if (mode & RISING_EDGE) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_RISINGDETECT);
    } else {
	rmw_clear(1 << gpio, addr + AM335X_GPIO_RISINGDETECT);
    }

    if (mode & FALLING_EDGE) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_FALLINGDETECT);
    } else {
	rmw_clear(1 << gpio, addr + AM335X_GPIO_FALLINGDETECT);
    }

    if (mode & LEVEL_DETECT0) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_LEVELDETECT0);
    } else {
	rmw_clear(1 << gpio, addr + AM335X_GPIO_LEVELDETECT0);
    }

    if (mode & LEVEL_DETECT1) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_LEVELDETECT1);
    } else {
	rmw_clear(1 << gpio, addr + AM335X_GPIO_LEVELDETECT1);
    }


    return 0;
}

    
#endif /* EXCEPTION && IRQ */

void gpio_set_value(short bank, short gpio)
{
    uint32_t addr, direction;

    addr = gpio_bank[bank];

    direction = readl(addr + AM335X_GPIO_OE) & (1 << gpio);

    if (!direction) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_DATASET);
    }

    return ;
}

void gpio_clear_value(short bank, short gpio)
{
    uint32_t addr, direction;

    addr = gpio_bank[bank];

    direction = readl(addr + AM335X_GPIO_OE) & (1 << gpio);

    if (!direction) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_DATACLEAR);
    }

    return ;
}

int gpio_get_value(short bank, short gpio)
{
    uint32_t addr, direction;

    addr = gpio_bank[bank];

    direction = readl(addr + AM335X_GPIO_OE) & (1 << gpio);

#ifdef DEBUG_GPIO
    printf("addr=%p, direction=%s, value=%p\n", addr, 
			readl(addr + AM335X_GPIO_OE) & (1 << gpio) ? "in" : "out",
			readl(addr + AM335X_GPIO_OE) & (1 << gpio) ? 
			(readl(addr + AM335X_GPIO_DATAIN) & (1 << gpio)) >> gpio : 
			(readl(addr + AM335X_GPIO_DATAOUT) & (1 << gpio)) >> gpio);
#endif

    if (direction) {
	return (readl(addr + AM335X_GPIO_DATAIN) & (1 << gpio)) >> gpio;
    } else {
	return (readl(addr + AM335X_GPIO_DATAOUT) & (1 << gpio)) >> gpio;
    }

    return -1;
}

void gpio_oe_enable(short bank, short gpio, uint8_t direction)
{
    uint32_t addr;

    addr = gpio_bank[bank];

#ifdef DEBUG_GPIO
    printf("addr=%p gpio=%p\n", addr, gpio);
    printf("value=%p, addr=%p\n", 1 << gpio, addr + AM335X_GPIO_OE);
#endif

    if (direction) {
	rmw_set(1 << gpio, addr + AM335X_GPIO_OE);
    } else {
	rmw_clear(1 << gpio, addr + AM335X_GPIO_OE);
    }

#ifdef DEBUG_GPIO
    printf("value=%p\n", (readl(addr + AM335X_GPIO_OE)));
#endif

    return ;
}

#endif /* GPIO */
