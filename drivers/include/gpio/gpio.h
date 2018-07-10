#ifndef _GPIO
#include <asm/types.h>

void gpio_direction_out(int);
void gpio_direction_in(int);
void gpio_set(int);
void gpio_clear(int);
int gpio_get(int);
int gpio_irq(int, int);
void gpio_oe_enable(short, short, uint8_t);
void gpio_set_value(short, short);
void gpio_clear_value(short, short);
int gpio_get_value(short, short);
int _gpio_irq(int, int, int);

#define RISING_EDGE 0x00000001
#define FALLING_EDGE	0x00000002
#define LEVEL_DETECT0	0x00000004
#define LEVEL_DETECT1	0x00000008

#endif /* _GPIO */
