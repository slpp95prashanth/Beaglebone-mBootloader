.text

print_ch:
    ldr r3, =0x44e09014
LOOP: ldrb r4, [r3]
    ands r4, #0x20
    bne LOOP
    ldr r3, =0x44e09000
    strb r0, [r3]
    mov pc, lr

.globl asm_memset
asm_memset:
.word 
    PUSH {lr}
    PUSH {r3 - r6}
    ldr r3, =0x00000000

    orr r3, r1, r3
    lsl r1, r1, #8
    orr r3, r1, r3
    lsl r1, r1, #8
    orr r3, r1, r3
    lsl r1, r1, #8
    orr r3, r1, r3

    mov r4, r3
    mov r5, r3
    mov r6, r3

#   mov r3, r7
LDMIA_MEMSET_LOOP: stm r0!, {r3 - r6}
    subs r2, r2, #16
    bne LDMIA_MEMSET_LOOP
    POP {r3 - r6}
    POP {lr}
    mov pc, lr

.globl asm_memcpy
asm_memcpy:
    PUSH {r3 - r6}
LDMIA_LOOP:
    ldmia r1!, {r3 - r6}
    stmia r0!, {r3 - r6}
    subs  r2, r2, #16
    bge   LDMIA_LOOP
    POP {r3 - r6}
    mov pc, lr

.globl asm_getregs
asm_getregs:
    push {r1}
    stmia r0, {r0 - r15}
    mrs r1, cpsr
    str r1, [r0, #64]
    pop {r1}
    mov pc, lr
/*
# Entry  r0: numerator (lo) must be signed positive
#        r2: deniminator (den) must be non-zero and signed negative
.globl idiv
idiv:
        lo .req r0; hi .req r1; den .req r2
        mov hi, #0 ;@ hi = 0
        adds lo, lo, lo
        .rept 32 ;# repeat 32 times
          adcs hi, den, hi, lsl #1
          subcc hi, hi, den
          adcs lo, lo, lo
        .endr
        mov pc, lr ;#return
# Exit   r0: quotient (lo)
#        r1: remainder (hi)

.globl imod
imod:
        lo .req r0; hi .req r1; den .req r2
        mov hi, #0 ;@ hi = 0
        adds lo, lo, lo
        .rept 32 ;# repeat 32 times
          adcs hi, den, hi, lsl #1
          subcc hi, hi, den
          adcs lo, lo, lo
        .endr
	mov r0, r1
        mov pc, lr ;#return
*/
