#include <asm/io.h>
#include <lib/irq.h>
#include <stdio.h>
#include <gpio/gpio.h>
#include <am335x-timer.h>
#include <am335x-irq.h>
#include <am335x-gpio.h>
#include <timer/timer.h>

#ifdef TIMER

#if defined(EXCEPTION) && defined(IRQ)

static struct timer _timer[MAX_TIMER_IN_USE];

static int __count = 0;

void request_timer(struct timer *data)
{
	if (data == NULL || data->func == NULL || data->usecs < DEFAULT_TIMER_USECS) {
		printf("invalid struct timer members\n");
		return ;
	}

	if (__count >= MAX_TIMER_IN_USE) {
		printf("reached max timer registered functions\n");
		return;
	}

	_timer[__count].usecs = data->usecs;
	_timer[__count].func = data->func;

	__count++;
}

void timer_init(int irq, void *usecs)
{
	unsigned int loader;
	int i, ret = 0;

	/* ACK interrupt */
	writel(OVF_EN_FLAG, AM335X_DMTIMER0_IRQSTATUS);

	for (i = 0; i <= __count; i++) {
		_timer[i].usecs -= DEFAULT_TIMER_USECS;

		if (_timer[i].usecs <= 0 && _timer[i].func != NULL) {
			ret = _timer[i].func(NULL);

			if (ret != TIMER_STOP) {
				_timer[i].usecs = ret;
			}
		}
	}

#ifdef TIMER_GPIO_DEBUG

	gpio_direction_out(USER_LED0);

	if (gpio_get(USER_LED0) == (1)) {
		gpio_clear(USER_LED0);
	} else {
		gpio_set(USER_LED0);
	}

#endif /* TIMER_GPIO_DEBUG */

	/* Required fplib */
//	loader = (unsigned int)(0xffffffff - ((float)AM335X_DMTIMER0_CLK * ((float)(*(unsigned int *)usecs) / 1000000)));

	/* 10 ms timer */
	loader = 0xfffffec0;

	writel(loader, AM335X_DMTIMER0_TLDR);
	writel(loader, AM335X_DMTIMER0_TCRR);

	writel(OVF_EN_FLAG, AM335X_DMTIMER0_IRQENABLE_SET);
}

#if TIMER_DEBUG
int print_10ms(void *data)
{
	printf("10ms\n");
	return 40000;
}

int print_20ms(void *data)
{
	printf("20ms\n");
	return 20000;
}

int print_30ms(void *data)
{
	printf("30ms\n");
	return 30000;
}

#endif

void timer_irq_init(void)
{
	timer_init(0, NULL);

	struct timer data;
#if TIMER_DEBUG
	data.usecs = 20000;
	data.func = print_20ms;
	request_timer(&data);
	printf("__count = %x line = %x\n", __count, __LINE__);
	data.usecs = 30000;
	data.func = print_30ms;
	request_timer(&data);
	printf("__count = %x line = %x\n", __count, __LINE__);
	data.usecs = 10000;
	data.func = print_10ms;
	request_timer(&data);
	printf("__count = %x line = %x\n", __count, __LINE__);
#endif

	request_irq(TIMER0_INTR, (int (*)(int, void *))timer_init, (void *)DEFAULT_TIMER_USECS);

	writel(START_TIMER | AUTO_RELOAD | TRIGGER_ON_OVERFLOW, AM335X_DMTIMER0_TLCR);
}

#endif /* EXCEPTION && IRQ */

#endif  /* TIMER */
