#include<lib/io.h>
#include<cmd.h>
#include<lib/string.h>

#ifdef SHELL

int do_regdump(void)
{
    puts("do_regdump\n");
    return 0;
}

void shell_start(void)
{
    char cmd[MAX_CMD_LEN];
    int ret = -1;

    while (1) {
	puts("mBoot> ");
	gets(cmd);

	if (CMD_CMP(cmd, "regdump") == (0)) {
	   ret = do_regdump();
	} else {
	    puts("unknown command\n");
	}

	if (ret == -1)
	    memset(cmd, '\0', MAX_CMD_LEN);
    }

    /* control does not reach here */
    return;
}

#endif /* SHELL */


 
