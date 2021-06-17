#include <stdio.h>
#include <lib/io.h>
#include "cmd.h"
#include <asm/regs.h>
#include <asm/io.h>
#include <gpio/gpio.h>
#include <asm/prcm.h>
#include <lib/irq.h>

int check_address_validity(char *);

#ifdef SHELL_REGDUMP

int do_regdump(int argc, char *argv[])
{
    int i;

    char str[12] = {};
    struct regs ptregs;

    asm_getregs(&ptregs);

    for (i = 0;i <= 16;i++) {
	inttostr(*(int *)((unsigned int *)&ptregs + i), (str), 16);
	puts("0x");
	puts(str);
	puts("\n");
    }

    return 0;
}

#endif /* SHELL_REGDUMP */
#ifdef SHELL_MD

int do_md(int argc, char *argv[])
{
	if (argc != 2) {
		puts("syntax: md <address>\n");
		return -1;
	}

	char *addr = argv[1];

	unsigned int address, data;

	if (check_address_validity(argv[1]) == -1) {
    		puts("invalid address\n");
    		return -1;
	}

	address = simple_strtoul(addr, NULL, 16);

	data = readl(address);

#ifndef DEBUG_PRINTF
    	char str[120] = {};
    	inttostr(data, str, 16);

    	puts(addr);
    	puts(" = ");
    	puts(str);
    	puts("\n");
#else
    	printf("%p = %p\n", address, data);
#endif

    	return 0;
}

#endif /* SHELL_MD */

#ifdef SHELL_MM

int do_mm(int argc, char *argv[])
{
	if (argc != 3) {
		puts("syntax: mm <address> <data>\n");
		return -1;
	}

    	unsigned int address, data;

	if (check_address_validity(argv[1]) == -1) {
    		puts("invalid address\n");
    		return -1;
	}


    	address = simple_strtoul(argv[1], NULL, 16);

    	data = simple_strtoul(argv[2], NULL, 16);

    	writel(data, address);

    	data = readl(address);

#ifdef DEBUG_PRINTF

    	printf("%p = %x\n", address, data);
#else

    	char str[120] = {};

    	inttostr(data, str, 16);

    	puts(addr);
    	puts(" = ");
    	puts(str);
    	puts("\n");

#endif /* DEBUG_PRINTF */

   	return 0;
}

#endif /* SHELL_MM */

#if defined(GPIO) && defined(SHELL_GPIO)

int do_gpio(int argc, char **argv)
{
#ifdef NET
	extern void cpsw_send(char *, uint32_t);
	cpsw_send(NULL, 0);
#endif
	return 0;

	if (argc != 3) {
		goto err;
	}

	int gpio;

	gpio = simple_strtoul(argv[(2)], NULL, 10);

	if (CMD_CMP(argv[1], "set") == 0) {
		gpio_set(gpio);
	} else if (CMD_CMP(argv[1], "clear") == 0) {
		gpio_clear(gpio);
	} else if (CMD_CMP(argv[1], "in") == 0) {
		gpio_direction_in(gpio);
	} else if (CMD_CMP(argv[1], "out") == 0) {
		gpio_direction_out(gpio);
	} else if (CMD_CMP(argv[1], "get") == 0) {
#ifdef DEBUG_PRINTF
		printf("gpio %p = %p\n", gpio, (gpio_get(gpio)));
#endif /* DEBUG_PRINTF */
#if defined(EXCEPTION) && defined(IRQ)
	} else if (CMD_CMP(argv[1], "irq") == 0) {
		gpio_irq(gpio, RISING_EDGE);
#endif
	} else {
		goto err;
	}

	return 0;

err:
	puts("Usage: gpio <[set] [clear] [get] [in] [out] ");
#if defined(EXCEPTION) && defined(IRQ)
	puts("[irq]> ");
#endif
	puts("<gpio pin>\n");

	return -1;
}
#endif /* GPIO && SHELL_GPIO */

#ifdef SHELL_RESET
int do_reset(int argc, char *argv[])
{
	if (argc == 2 && strncmp(argv[1], "cold", 4) == 0) {
		reset_cpu(COLD_RESET);
	} else if (argc == 1) {
		reset_cpu(WARM_RESET);
	} else {
		printf("syntax: reset [cold]\n");
		return -1;
	}

	return 0;
}
#endif /* SHELL_RESET */

int do_intr(int argc, char *argv[])
{
	if (argc == 1) {
		print_irq_enabled();
		print_irq_count();

		return 0;
	} else if (argc == 2) {
		if (strncmp(argv[1], "enable", 6) == 0) {
			enable_irq();
		} else if (strncmp(argv[1], "disable", 7) == 0) {
			disable_irq();
		}

		return 0;
	}

	return -1;
}

extern void system_uptime(void);

int do_time(int argc, char *argv[])
{
	system_uptime();

	return 0;
}
