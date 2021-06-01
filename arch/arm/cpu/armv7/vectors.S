#ifdef EXCEPTION

#define WARM_RESET 0x01

.section "vectors", "ax"

.align 4

.globl EXP_STACK_START
EXP_STACK_START:
        .word   EXP_STACK_START_IN			@ Exception-mode stack

.macro asm_getregs
    stmia r13, {r0 - r15}
.endm

.align 4
undefined_instruction:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	sp, sp, #128
	stmia	sp, {r0 - r15}			@ store r0 - r12 to stack
	mrs	r6, cpsr
	str	r6, [sp, #64]
	mov	r0, sp
        bl      _regs

.align 4
software_interrupt:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	lr, lr, #4
	stmfd	sp!, {r0 - r12, r14}
	mrs	r6, spsr
	orr	r6, r6, #0x00000080		@ disable I bit for irq to be triggered
	msr	spsr, r6
#ifdef IRQ
#if defined(COREDUMP) && defined(DEBUG_IRQ)
	stmfd	sp!, {r6}
	bl	irq_showregs
	ldmfd	sp!, {r6}
#endif
	bl	do_irq
#endif
	ldmfd	sp!, {r0 - r12, pc}^

.align 4
prefetch_abort:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	sp, sp, #128
	stmia	sp, {r0 - r15}			@ store r0 - r12 to stack
	mrs	r6, cpsr
	str	r6, [sp, #64]
	mov	r0, sp
        bl      _regs

.align 4
data_abort:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	sp, sp, #128
	stmia	sp, {r0 - r15}			@ store r0 - r12 to stack
	mrs	r6, cpsr
	str	r6, [sp, #64]
	mov	r0, sp
        bl      _regs

.align 4
not_used:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	sp, sp, #128
	stmia	sp, {r0 - r15}			@ store r0 - r12 to stack
	mrs	r6, cpsr
	str	r6, [sp, #64]
	mov	r0, sp
        bl      _regs

.align 4
irq:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	lr, lr, #4
	stmfd	sp!, {r0 - r12, r14}
	mrs	r6, spsr
	orr	r6, r6, #0x00000080		    @ disable I bit for irq to be triggered
	msr	spsr, r6
#ifdef IRQ
#if defined(COREDUMP) && defined(DEBUG_IRQ)
	stmfd	sp!, {r6}
	bl	irq_showregs
	ldmfd	sp!, {r6}
#endif
	bl	do_irq
#endif
	ldmfd	sp!, {r0 - r12, pc}^

.align 4
fiq:
        ldr     sp, =EXP_STACK_START_IN         @ setup our mode stack
	sub	sp, sp, #128
	stmia	sp, {r0 - r15}			@ store r0 - r12 to stack
	mrs	r6, cpsr
	str	r6, [sp, #64]
	mov	r0, sp
        bl      _regs

#define CR_V    (1 << 13)       /* Vectors relocated to 0xffff0000      */

set_exception_vectors:
        /* Set V=0 in CP15 SCTRL register - for VBAR to point to vector */
        mrc     p15, 0, r0, c1, c0, 0   @ Read CP15 SCTRL Register
        bic     r0, #CR_V               @ V = 0
        mcr     p15, 0, r0, c1, c0, 0   @ Write CP15 SCTRL Register

        /* Set vector address in CP15 VBAR register */
        ldr     r0, =__exception_vector_table
        mcr     p15, 0, r0, c12, c0, 0  @Set VBAR
        mov pc, lr

.globl cpu_enable_intr
cpu_enable_intr:
    push {r6}
    mrs	r6, cpsr
    bic r6, r6, #0x00000080		    @ disable I bit for irq to be triggered
#    ldr r6, =0x60000113
    msr	cpsr, r6
    pop	{r6}
    mov	pc, lr

.globl cpu_disable_intr
cpu_disable_intr:
    push {r6}
    mrs	r6, cpsr
    orr r6, r6, #0x00000080		    @ disable I bit for irq to be triggered
#    ldr r6, =0x60000113
    msr	cpsr, r6
    pop	{r6}
    mov	pc, lr

.globl cpu_irq_init_pre_mode_spsr
cpu_irq_init_pre_mode_spsr:
    push {r6}
    mrs	r6, spsr
    bic r6, r6, #0x00000080		    @ disable I bit for irq to be triggered
#    ldr r6, =0x60000113
    msr	spsr, r6
    pop	{r6}
    mov	pc, lr

.globl asm_exception
asm_exception:
	push {lr}
	bl set_exception_vectors
	pop {lr}
        mov pc, lr

.align 4
#.globl __exception_vector_table
__exception_vector_table:
	nop
        ldr     pc, _undefined_instruction
        ldr     pc, _software_interrupt
        ldr     pc, _prefetch_abort
        ldr     pc, _data_abort
        ldr     pc, _not_used
        ldr     pc, _irq
        ldr     pc, _fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt:    .word software_interrupt
_prefetch_abort:        .word prefetch_abort
_data_abort:            .word data_abort
_not_used:              .word not_used
_irq:                   .word irq
_fiq:                   .word fiq
_pad:                   .word 0x12345678  /* now 16*4=64 */

#endif /* EXCEPTION */
