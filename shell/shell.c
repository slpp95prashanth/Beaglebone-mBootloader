#include <lib/io.h>
#include <cmd.h>
#include <lib/string.h>
#include <asm/types.h>
#include <stdio.h>
#include <asm/io.h>
#include <asm/regs.h>
#include <asm/prcm.h>
#include <gpio/gpio.h>
#include <lib/irq.h>

#ifdef SHELL
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
#ifdef IRQ
    } else if (CMD_CMP(argv[1], "irq") == 0) {

	gpio_irq(gpio, RISING_EDGE);
#endif
    } else {
	puts("Usage: gpio <[set] [clear] [in] [out] ");
#ifdef IRQ
	puts("[irq]> ");
#endif
	puts("<gpio pin>\n");

	return -1;
    }

    return 0;
}

#endif /* GPIO && SHELL_GPIO */

void shell_start(void)
{
    char cmd[MAX_CMD_LEN] = {}, *argv[MAX_CMD_ARGS];
    int ret, argc;

    ret = -1;

    while (1) {
	puts("mBoot> ");
	gets(cmd);

	argc = 0;

	argv[argc] = strtok(cmd, " ");
	
	while (argv[argc] != (NULL)) {
	    argv[++argc] = strtok(NULL, " ");
	}

	if (CMD_CMP(cmd, "regdump") == (0)) {
#ifdef SHELL_REGDUMP
	   ret = do_regdump();
#endif /* SHELL_REGDUMP */

	} else if (CMD_CMP(cmd, "md") == (0)) {
#ifdef SHELL_MD
	    if (argc != 2) {
		puts("syntax: md <address>\n");
	    } else {
		ret = do_md(argv[(1)]);
	    }
#endif /* SHELL_MD */
	} else if (CMD_CMP(cmd, "mm") == (0)) {
#ifdef SHELL_MM
	    if (argc != 3) {
		puts("syntax: mm <address> <data>\n");
	    } else {
		ret = do_mm(argv[1], argv[2]);
	    }
#endif /* SHELL_MD */
	} else if (CMD_CMP(cmd, "reset") == (0)) {
#ifdef SHELL_RESET
	    if (argc == 2 && CMD_CMP(argv[1], "cold") == 0) {
	    	reset_cpu(COLD_RESET);
	    } else {
	    	reset_cpu(WARM_RESET);
	    }
#endif /* SHELL_RESET */
	} else if (CMD_CMP(cmd, "gpio") == (0)) {
#if defined(GPIO) && defined(SHELL_GPIO)
	    if (argc != 3) {
		puts("Usage: gpio <[set] [clear] [in] [out] ");
#ifdef IRQ
		puts("[irq]> ");
#endif
		puts("<gpio pin>\n");
	    } else {
		ret = do_gpio(argc, argv);
	    }
#endif /* GPIO && SHELL_GPIO */
	} else {
	    puts("unknown command\n");
	}

	if (ret < 0) {
	    memset(cmd, '\0', MAX_CMD_LEN);
	}
    }

    /* control does not reach here */
    return;
}

#endif /* SHELL */


 
