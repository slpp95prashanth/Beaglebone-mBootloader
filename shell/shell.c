#include <lib/io.h>
#include "cmd.h"
#include <lib/string.h>
#include <asm/types.h>
#include <stdio.h>

#define BACKSPACE	putc(' ');putc('\b');

#ifdef SHELL

char cmds[][MAX_CMD_LEN] = {	\
		"md",	\
		"mm",	\
		"regdump",	\
		"ethstats",	\
		"gpio",	\
		"reset",	\
		""
		};

void do_help(void)
{
	int i;

	for(i = 0; strlen(cmds[i]) != 0; i++) {
		printf("%s\n", cmds[i]);
	}

	printf("\n");
}

void cmd_tab_completion(char *cmd, int i)
{
	int j;

	printf("\n");

	for (j = 0; strlen(cmds[j]) != 0; j++) {
		if (strncmp(cmd, cmds[j], i)  == 0) {
			printf("%s\n", cmds[j]);
		}
	}
}
			
void shell_start(void)
{
	char cmd[MAX_CMD_LEN] = {}, *argv[MAX_CMD_ARGS], ch;
	int ret, argc, i;

	ret = -1;

	while (1) {
		puts("mBoot> ");

		i = 0;

		while (ch != '\r' || ch != '\n') {
			ch = getc();
			putc(ch);

			if (ch == '\b') {
				BACKSPACE;

				if (i > 0) {
					i--;
				}

			} else if (ch == '\r' || ch == '\n') {
				putc('\n');

				cmd[i] = '\0';
				break;
			} else if (ch == '\t') {		/* tab completion */
				cmd_tab_completion(cmd, i);
			} else {
				cmd[i++] = ch;
			}
		}

		argc = 0;

		argv[argc] = strtok(cmd, " ");
	
		while (argv[argc] != (NULL)) {
			argv[++argc] = strtok(NULL, " ");
		}

		if (CMD_CMP(cmd, "regdump") == (0)) {
#ifdef SHELL_REGDUMP
	   	ret = do_regdump(argc, argv);
#endif /* SHELL_REGDUMP */
#ifdef SHELL_MD
		} else if (CMD_CMP(cmd, "md") == (0)) {
			ret = do_md(argc, argv);
#endif /* SHELL_MD */
#ifdef SHELL_MM
		} else if (CMD_CMP(cmd, "mm") == (0)) {
			ret = do_mm(argc, argv);
#endif /* SHELL_MM */
#ifdef SHELL_RESET
		} else if (CMD_CMP(cmd, "reset") == (0)) {
			do_reset(argc, argv);
#endif /* SHELL_RESET */
#if defined GPIO && defined SHELL_GPIO
		} else if (CMD_CMP(cmd, "gpio") == (0)) {
			ret = do_gpio(argc, argv);
#endif /* GPIO && SHELL_GPIO */
#ifdef NET
		} else if (CMD_CMP(cmd, "ethstats") == (0)) {
			do_eth_stats();
		} else if (CMD_CMP(cmd, "intr") == (0)) {
			do_intr(argc, argv);
#endif /* NET */
		} else if (CMD_CMP(cmd, "help") == (0) || CMD_CMP(cmd, "?") == (0)) {
			do_help();
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


 
