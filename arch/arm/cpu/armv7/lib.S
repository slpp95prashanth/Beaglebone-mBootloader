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
    ldmia r0, {r3 - r6}
    stmia r1, {r3 - r6}
    subs  r2, r2, #16
    bge   LDMIA_LOOP
    POP {r3 - r6}
    mov pc, lr



 
