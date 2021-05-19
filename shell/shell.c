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
#if defined(EXCEPTION) && defined(IRQ)
		puts("[irq]> ");
#endif
		puts("<gpio pin>\n");
	    } else {
		ret = do_gpio(argc, argv);
	    }
#endif /* GPIO && SHELL_GPIO */
	} else if (CMD_CMP(cmd, "ethstats") == (0)) {
	    if (argc != 1) {
		puts("syntax: ethstats\n");
	    } else {
		ret = do_eth_stats();
	    }
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


 
