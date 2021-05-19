#include <stdio.h>
#include <cmd.h>
#include <asm/regs.h>
#include <asm/io.h>
#include <gpio/gpio.h>

#ifdef SHELL_REGDUMP

int do_regdump(void)
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

int do_md(char *addr)
{
    unsigned int address, data, len;

    len = strlen(addr);

    if (len < 8) {
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

int do_mm(char *addr, char *data)
{
    unsigned int address, reg;

    if (strlen(addr) < 8 && strlen(data) > 8) {
	puts("invalid argument\n");
	return -1;
    }

    address = simple_strtoul(addr, NULL, 16);

    reg = simple_strtoul(data, NULL, 16);

    writel(reg, address);
    writel(reg, address);

    reg = readl(address);

#ifdef DEBUG_PRINTF

    tfp_printf("%p = %x\n", address, reg);
#else

    char str[120] = {};

    inttostr(reg, str, 16);

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
    int gpio;

    cpsw_recv();

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

	printf("gpio %p = %p\n", gpio, (gpio_get(gpio)));
#if defined(EXCEPTION) && defined(IRQ)
    } else if (CMD_CMP(argv[1], "irq") == 0) {

	gpio_irq(gpio, RISING_EDGE);
#endif
    } else {
	puts("Usage: gpio <[set] [clear] [in] [out] ");
#if defined(EXCEPTION) && defined(IRQ)
	puts("[irq]> ");
#endif
	puts("<gpio pin>\n");

	return -1;
    }

    return 0;
}

#endif /* GPIO && SHELL_GPIO */

