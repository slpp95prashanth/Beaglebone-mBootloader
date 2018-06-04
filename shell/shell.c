#include<lib/io.h>
#include<cmd.h>
#include<lib/string.h>
#include<asm/types.h>
#include<stdio.h>

struct regs {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t r13;
    uint32_t r14;
    uint32_t r15;
};

void asm_getregs(struct regs *);

#ifdef SHELL
#ifdef SHELL_REGDUMP

int do_regdump(void)
{

#define print_regs(regs, str)	\
inttostr(regs, str, 16);\
puts("0x");			\
puts(str);			\
puts("\n");			\

    char str[12] = {};
    struct regs ptregs;

    asm_getregs(&ptregs);

    print_regs(ptregs.r0, str);
    print_regs(ptregs.r1, str);
    print_regs(ptregs.r2, str);
    print_regs(ptregs.r3, str);
    print_regs(ptregs.r4, str);
    print_regs(ptregs.r5, str);
    print_regs(ptregs.r6, str);
    print_regs(ptregs.r7, str);
    print_regs(ptregs.r8, str);
    print_regs(ptregs.r9, str);
    print_regs(ptregs.r10, str);
    print_regs(ptregs.r11, str);
    print_regs(ptregs.r12, str);
    print_regs(ptregs.r13, str);
    print_regs(ptregs.r14, str);
    print_regs(ptregs.r15, str);

    return 0;
}

#endif /* SHELL_REGDUMP */

#ifdef SHELL_MD

int do_md(int argc, char **argv)
{
    char str[120] = {};

    inttostr(argc, str, 16);

    puts(str);
    putc('\n');

    while (argc > 0) {
        argc--;
        puts(argv[(argc)]);
	putc('\n');
    }

    return 0;
}

#endif /* SHELL_MD */

void shell_start(void)
{
    char cmd[MAX_CMD_LEN], *argv[MAX_CMD_ARGS];
    int ret, argc;

    ret = -1;

    while (1) {
	puts("mBoot> ");
	gets(cmd);

	argc = 0;

	argv[argc] = strtok(cmd, " ");

	while (argv[argc] != (NULL))
	    argv[++argc] = strtok (NULL, " ");

	if (CMD_CMP(cmd, "regdump") == (0)) {
#ifdef SHELL_REGDUMP
	   ret = do_regdump ();
#endif /* SHELL_REGDUMP */

	} else if (CMD_CMP(cmd, "md") == (0)) {
#ifdef SHELL_MD
	   ret = do_md (argc, argv);
#endif /* SHELL_MD */
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


 
