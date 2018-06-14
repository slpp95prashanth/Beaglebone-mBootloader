#ifdef EXCEPTION

#define WARM_RESET 0x01

.section "vectors", "ax"

.align 4

#.globl EXP_STACK_START_IN
#EXP_STACK_START_IN:
#        .word   0x402ff000			@ Exception-mode stack

undefined_instruction:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr

software_interrupt:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr

prefetch_abort:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr
data_abort:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr

not_used:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr
irq:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr
fiq:
        ldr     r13, =EXP_STACK_START_IN         @ setup our mode stack
	push	{r0, lr}
        bl      do_regdump
	ldr	r0, =WARM_RESET
	bl	reset_cpu
	pop	{r0, lr}
        mov pc, lr
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


.globl asm_exception
asm_exception:
	push {lr}
	bl set_exception_vectors
	pop {lr}
        mov pc, lr

.globl __exception_vector_table
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
