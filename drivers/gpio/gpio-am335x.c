#include <asm/io.h>
#include <asm/types.h>
#include <am335x-gpio.h>
#include <stdio.h>

#ifdef GPIO

static uint32_t gpio_bank[] = {AM335x_GPIO0_BASE, AM335x_GPIO1_BASE};

void gpio_set_value(short bank, short gpio)
{
    uint32_t addr, direction;

    addr = gpio_bank[bank];

    direction = readl(addr + AM335X_GPIO_OE) & (1 << gpio);

    if (!direction) {
	writel(1 << gpio, addr + AM335X_GPIO_DATASET);
    }

    return ;
}

void gpio_clear_value(short bank, short gpio)
{
    uint32_t addr, direction;

    addr = gpio_bank[bank];

    direction = readl(addr + AM335X_GPIO_OE) & (1 << gpio);

    if (!direction) {
	writel(1 << gpio, addr + AM335X_GPIO_DATACLEAR);
    }

    return ;
}
 
void gpio_oe_enable(short bank, short gpio, uint8_t direction)
{
    uint32_t addr;

    addr = gpio_bank[bank];

    writel(direction << gpio, addr + AM335X_GPIO_OE);

    return ;
}

#endif /* GPIO */
