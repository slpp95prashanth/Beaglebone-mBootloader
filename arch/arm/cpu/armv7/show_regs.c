#include <asm/regs.h>
#include <asm/prcm.h>

#include <lib/string.h>
#include <stdio.h>

#ifdef EXCEPTION

#define MODE_MASK       0x1f
#define T_BIT           0x20
#define F_BIT           0x40
#define I_BIT           0x80
#define CC_V_BIT        (1 << 28)
#define CC_C_BIT        (1 << 29)
#define CC_Z_BIT        (1 << 30)
#define CC_N_BIT        (1 << 31)

#define processor_mode(regs) \
        ((regs)->cpsr & MODE_MASK)

#define interrupts_enabled(regs) \
        (!((regs)->cpsr & I_BIT))

#define fast_interrupts_enabled(regs) \
        (!((regs)->cpsr & F_BIT))

#define condition_codes(regs) \
        ((regs)->cpsr & (CC_V_BIT|CC_C_BIT|CC_Z_BIT|CC_N_BIT))

#define thumb_mode(regs) \
        (((regs)->cpsr & T_BIT))

#ifdef CRASHDUMP 

void show_regs (struct regs *regs)
{
        unsigned long flags;
        const char *processor_modes[] = {
        "USER_26",      "FIQ_26",       "IRQ_26",       "SVC_26",
        "UK4_26",       "UK5_26",       "UK6_26",       "UK7_26",
        "UK8_26",       "UK9_26",       "UK10_26",      "UK11_26",
        "UK12_26",      "UK13_26",      "UK14_26",      "UK15_26",
        "USER_32",      "FIQ_32",       "IRQ_32",       "SVC_32",
        "UK4_32",       "UK5_32",       "UK6_32",       "ABT_32",
        "UK8_32",       "UK9_32",       "UK10_32",      "UND_32",
        "UK12_32",      "UK13_32",      "UK14_32",      "SYS_32",
        };

        flags = condition_codes (regs);

#ifdef DEBUG_PRINTF

        printf ("pc : [<%08lx>]    lr : [<%08lx>]\n"
                "sp : %08lx  ip : %08lx  fp : %08lx\n",
		regs->r15, regs->r14, regs->r13, regs->r15, regs->r12);
        printf ("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
                regs->r10, regs->r9, regs->r8);
        printf ("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
                regs->r7, regs->r6, regs->r5, regs->r4);
        printf ("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
                regs->r3, regs->r2, regs->r1, regs->r0);
        printf ("Flags: %c%c%c%c",
                flags & CC_N_BIT ? 'N' : 'n',
                flags & CC_Z_BIT ? 'Z' : 'z',
                flags & CC_C_BIT ? 'C' : 'c', flags & CC_V_BIT ? 'V' : 'v');
        printf ("  IRQs %s  FIQs %s  Mode %s%s\n",
                interrupts_enabled (regs) ? "on" : "off",
                fast_interrupts_enabled (regs) ? "on" : "off",
                processor_modes[processor_mode (regs)],
                thumb_mode (regs) ? " (T)" : " ");

#endif /* DEBUG_PRINTF */

	reset_cpu(0);

	return;
}

#endif /* CRASHDUMP */

/*
void show_regs(struct regs *ptregs)
{
    int i;
    char str[16];

    printf("%s ptregs=%p\n", __func__, ptregs);

    for (i = 0;i < 16;i++) {
#ifndef DEBUG_PRINTF
	inttostr(*(int *)((unsigned int *)ptregs + i), (str), 16);
	puts("0x");
	puts(str);
	puts("\n");
#else
	printf("r%x=%p\n", i, *(unsigned int *)((unsigned int *)ptregs + i));
#endif
    }

	for (flags = 0;flags <= 31;flags++) {
	    printf("i=%x processor_modes=%s\n", flags, processor_modes[(flags)]);
	}
#ifndef DEBUG_PRINTF
    inttostr(*(int *)((unsigned int *)ptregs + i), (str), 16);
    puts("0x");
    puts(str);
    puts("\n");
#else
    printf("spsr=%p\n", *(unsigned int *)((unsigned int *)ptregs + i));
#endif
    return;
}
*/

#endif /* EXCEPTION */
