#include <gpio/gpio.h>
#include <am335x-gpio.h>
#include <lib/math.h>

#define GPIO_GET_BANK(gpio) \
    idiv_by_pow16(gpio, BANK_SIZE)

#define GPIO_GET_PIN(gpio)  \
    imod_less_than_32(gpio, BANK_SIZE - 1)

#ifdef GPIO

#ifdef IRQ

int gpio_irq(int gpio, int mode)
{
    int bank;

    bank = GPIO_GET_BANK(gpio);

    if (bank > MAX_BANK) {
	return -1;
    }
    
    return _gpio_irq(bank, (GPIO_GET_PIN(gpio)), mode);
}

#endif /* IRQ */

void gpio_direction_out(int gpio)
{
    int bank;

    bank = GPIO_GET_BANK(gpio);

    if (bank > MAX_BANK) {
	return ;
    }

    gpio_oe_enable(bank, (GPIO_GET_PIN(gpio)), OUT);
}

void gpio_direction_in(int gpio)
{
    int bank;

    bank = GPIO_GET_BANK(gpio);

    if (bank > MAX_BANK) {
	return ;
    }

    gpio_oe_enable(bank, (GPIO_GET_PIN(gpio)), IN);
}

void gpio_set(int gpio)
{
    int bank;

    bank = GPIO_GET_BANK(gpio);

    if (bank > MAX_BANK) {
	return ;
    }

    gpio_set_value(bank, (GPIO_GET_PIN(gpio)));
}

void gpio_clear(int gpio)
{
    int bank;

    bank = GPIO_GET_BANK(gpio);

    if (bank > MAX_BANK) {
	return ;
    }

    gpio_clear_value(bank, (GPIO_GET_PIN(gpio)));
}

int gpio_get(int gpio)
{
    int bank;

    bank = GPIO_GET_BANK(gpio);

    if (bank > MAX_BANK) {
        return -1;
    }

    return gpio_get_value(bank, (GPIO_GET_PIN(gpio)));
}

#endif /* GPIO */
