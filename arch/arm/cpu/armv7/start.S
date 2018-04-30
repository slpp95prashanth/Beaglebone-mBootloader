#.global SYMBOL_NAME(lowlevel_init)
.text
#.globl lowlevel_init
lowlevel_init:
        /*
         * Setup a temporary stack
         */
        ldr     sp, =0x4030E000
        bic     sp, sp, #7 /* 8-byte alignment for ABI compliance */

        /*
         * Save the old lr(passed in ip) and the current lr to stack
         */
        push    {ip, lr}

        /*
         * go setup pll, mux, memory
         */
        bl      board_init_f
        pop     {ip,pc}
#.type lowlevel_init STT_FUNC #;(lowlevel_init)

#.global SYMBOL_NAME(cpu_init_cp15)
cpu_init_cp15:
        /*
         * Invalidate L1 I/D
         */
        mov     r0, #0                  @ set up for MCR
        mcr     p15, 0, r0, c8, c7, 0   @ invalidate TLBs	p15(co_processor number) c8(TLB operations)   c7, 0 (invalidate TLBs)
        mcr     p15, 0, r0, c7, c5, 0   @ invalidate icache	p15(co_processor number) c7(cache operations) c5, 0 (invalidate icache)
        mcr     p15, 0, r0, c7, c5, 6   @ invalidate BP array	p15(co_processor number) c7(cache operations) c5, 6 (invalidate BP array)
        mcr     p15, 0, r0, c7, c10, 4  @ DSB
        mcr     p15, 0, r0, c7, c5, 4   @ ISB

        /*
         * disable MMU stuff and caches
         */
        mrc     p15, 0, r0, c1, c0, 0
        bic     r0, r0, #0x00002000     @ clear bits 13 (--V-)
        bic     r0, r0, #0x00000007     @ clear bits 2:0 (-CAM)
        orr     r0, r0, #0x00000002     @ set bit 1 (--A-) Align
        orr     r0, r0, #0x00000800     @ set bit 11 (Z---) BTB
#ifdef CONFIG_SYS_ICACHE_OFF
        bic     r0, r0, #0x00001000     @ clear bit 12 (I) I-cache
#else
        orr     r0, r0, #0x00001000     @ set bit 12 (I) I-cache
#endif
        mcr     p15, 0, r0, c1, c0, 0
        mov     pc, lr                  @ back to my caller
#.type cpu_init_cp15 STT_FUNC #;ENDPROC(cpu_init_cp15)

#.global SYMBOL_NAME(cpu_init_crit)
cpu_init_crit:
        /*
         * Jump to board specific initialization...
         * The Mask ROM will have already initialized
         * basic memory. Go here to bump up clock rate and handle
         * wake up conditions.
         */
        b       lowlevel_init           @ go setup pll,mux,memory
#.type cpu_init_crit STT_FUNC #;(cpu_init_crit)

#.global SYMBOL_NAME(_main)
.text
.globl _start
_start:	
/*
 * Set up initial C runtime environment and call board_init_f(0).
 */
	bl	cpu_init_cp15
	bl	cpu_init_crit
#        ldr     r0, 
#        bic     r0, r0, #7      /* 8-byte alignment for ABI compliance */
#        mov     sp, r0
#        mov     r0, #0
#        bl      board_init_f

#.type _main STT_FUNC #;(_main)

 
