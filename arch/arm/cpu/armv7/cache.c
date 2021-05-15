#include <asm/types.h>
#define CR_M    (1 << 0)        /* MMU enable                           */

#define CR_C    (1 << 2)        /* Dcache enable                        */
#define MIDR_PRIMARY_PART_MASK  0xFF0FFFF0

/* ID_PFR1 feature fields */
#define CPUID_ARM_SEC_SHIFT             4
#define CPUID_ARM_SEC_MASK              (0xF << CPUID_ARM_SEC_SHIFT)
#define CPUID_ARM_VIRT_SHIFT            12
#define CPUID_ARM_VIRT_MASK             (0xF << CPUID_ARM_VIRT_SHIFT)
#define CPUID_ARM_GENTIMER_SHIFT        16
#define CPUID_ARM_GENTIMER_MASK         (0xF << CPUID_ARM_GENTIMER_SHIFT)

/* valid bits in CBAR register / PERIPHBASE value */
#define CBAR_MASK                       0xFFFF8000

/* CCSIDR */
#define CCSIDR_LINE_SIZE_OFFSET         0
#define CCSIDR_LINE_SIZE_MASK           0x7
#define CCSIDR_ASSOCIATIVITY_OFFSET     3
#define CCSIDR_ASSOCIATIVITY_MASK       (0x3FF << 3)
#define CCSIDR_NUM_SETS_OFFSET          13
#define CCSIDR_NUM_SETS_MASK            (0x7FFF << 13)

/*
 * Values for InD field in CSSELR
 * Selects the type of cache
 */
#define ARMV7_CSSELR_IND_DATA_UNIFIED   0
#define ARMV7_CSSELR_IND_INSTRUCTION    1

/* Values for Ctype fields in CLIDR */
#define ARMV7_CLIDR_CTYPE_NO_CACHE              0
#define ARMV7_CLIDR_CTYPE_INSTRUCTION_ONLY      1
#define ARMV7_CLIDR_CTYPE_DATA_ONLY             2
#define ARMV7_CLIDR_CTYPE_INSTRUCTION_DATA      3
#define ARMV7_CLIDR_CTYPE_UNIFIED               4

#define CP15ISB asm volatile ("mcr     p15, 0, %0, c7, c5, 4" : : "r" (0))
#define CP15DSB asm volatile ("mcr     p15, 0, %0, c7, c10, 4" : : "r" (0))
#define CP15DMB asm volatile ("mcr     p15, 0, %0, c7, c10, 5" : : "r" (0))

#define ISB     CP15ISB
#define DSB     CP15DSB
#define DMB     CP15DMB
#define ARMV7_DCACHE_CLEAN_INVAL_ALL    2
#define ARMV7_DCACHE_INVAL_ALL          1
#define ARMV7_DCACHE_INVAL_RANGE        3
#define ARMV7_DCACHE_CLEAN_INVAL_RANGE  4

static inline int32_t log_2_n_round_up(uint32_t n)
{
        int32_t log2n = -1;
        uint32_t temp = n;
        
        while (temp) {
                log2n++;
                temp >>= 1;
        }

        if (n & (n - 1))
                return log2n + 1; /* not power of 2 - round up */
        else
                return log2n; /* power of 2 */
}       
static void v7_clean_inval_dcache_level_setway(uint32_t level, uint32_t num_sets,
                                               uint32_t num_ways, uint32_t way_shift,
                                               uint32_t log2_line_len)
{
        int way, set;
        uint32_t setway;

        /*
         * For optimal assembly code:
         *      a. count down
         *      b. have bigger loop inside
         */
        for (way = num_ways - 1; way >= 0 ; way--) {
                for (set = num_sets - 1; set >= 0; set--) {
                        setway = (level << 1) | (set << log2_line_len) |
                                 (way << way_shift);
                        /*
                         * Clean & Invalidate data/unified
                         * cache line by set/way
                         */
                        asm volatile (" mcr p15, 0, %0, c7, c14, 2"
                                        : : "r" (setway));
                }
        }
        /* DSB to make sure the operation is complete */
        DSB;
}

static void v7_inval_dcache_level_setway(uint32_t level, uint32_t num_sets,
                                         uint32_t num_ways, uint32_t way_shift,
                                         uint32_t log2_line_len)
{
        int way, set;
        uint32_t setway;

        /*
         * For optimal assembly code:
         *      a. count down
         *      b. have bigger loop inside
         */
        for (way = num_ways - 1; way >= 0 ; way--) {
                for (set = num_sets - 1; set >= 0; set--) {
                        setway = (level << 1) | (set << log2_line_len) |
                                 (way << way_shift);
                        /* Invalidate data/unified cache line by set/way */
                        asm volatile (" mcr p15, 0, %0, c7, c6, 2"
                                        : : "r" (setway));
                }
        }
        /* DSB to make sure the operation is complete */
        DSB;
}
#define nop() __asm__ __volatile__("mov\tr0,r0\t@ nop\n\t");
#define isb() __asm__ __volatile__ ("" : : : "memory")

static inline void set_cr(unsigned int val)
{
        asm volatile("mcr p15, 0, %0, c1, c0, 0 @ set CR"
          : : "r" (val) : "cc");
        isb();
}


static inline unsigned int get_cr(void)
{
        unsigned int val;
        asm("mrc p15, 0, %0, c1, c0, 0  @ get CR" : "=r" (val) : : "cc");
        return val;
}
static void cp_delay (void)
{
        volatile int i;

        /* copro seems to need some delay between reading and writing */
        for (i = 0; i < 100; i++)
                nop();
        asm volatile("" : : : "memory");
}
void flush_dcache_all(void) 
{
        v7_maint_dcache_all(ARMV7_DCACHE_CLEAN_INVAL_ALL);
}
uint32_t get_clidr(void)
{
        uint32_t clidr;

        /* Read current CP15 Cache Level ID Register */
        asm volatile ("mrc p15,1,%0,c0,c0,1" : "=r" (clidr));
        return clidr;
}
static void set_csselr(uint32_t level, uint32_t type)
{
        uint32_t csselr = level << 1 | type;

        /* Write to Cache Size Selection Register(CSSELR) */
        asm volatile ("mcr p15, 2, %0, c0, c0, 0" : : "r" (csselr));
}
static uint32_t get_ccsidr(void)
{
        uint32_t ccsidr;

        /* Read current CP15 Cache Size ID Register */
        asm volatile ("mrc p15, 1, %0, c0, c0, 0" : "=r" (ccsidr));
        return ccsidr;
}

static void v7_maint_dcache_level_setway(uint32_t level, uint32_t operation)
{
        uint32_t ccsidr;
        uint32_t num_sets, num_ways, log2_line_len, log2_num_ways;
        uint32_t way_shift;

        set_csselr(level, ARMV7_CSSELR_IND_DATA_UNIFIED);

        ccsidr = get_ccsidr();

        log2_line_len = ((ccsidr & CCSIDR_LINE_SIZE_MASK) >>
                                CCSIDR_LINE_SIZE_OFFSET) + 2;
        /* Converting from words to bytes */
        log2_line_len += 2;

        num_ways  = ((ccsidr & CCSIDR_ASSOCIATIVITY_MASK) >>
                        CCSIDR_ASSOCIATIVITY_OFFSET) + 1;
        num_sets  = ((ccsidr & CCSIDR_NUM_SETS_MASK) >>
                        CCSIDR_NUM_SETS_OFFSET) + 1;
        /*
         * According to ARMv7 ARM number of sets and number of ways need
         * not be a power of 2
         */
        log2_num_ways = log_2_n_round_up(num_ways);

        way_shift = (32 - log2_num_ways);
        if (operation == ARMV7_DCACHE_INVAL_ALL) {
                v7_inval_dcache_level_setway(level, num_sets, num_ways,
                                      way_shift, log2_line_len);
        } else if (operation == ARMV7_DCACHE_CLEAN_INVAL_ALL) {
                v7_clean_inval_dcache_level_setway(level, num_sets, num_ways,
                                                   way_shift, log2_line_len);
        }
}

void v7_maint_dcache_all(uint32_t operation)
{
        uint32_t level, cache_type, level_start_bit = 0;
        uint32_t clidr = get_clidr();

        for (level = 0; level < 7; level++) {
                cache_type = (clidr >> level_start_bit) & 0x7;
                if ((cache_type == ARMV7_CLIDR_CTYPE_DATA_ONLY) ||
                    (cache_type == ARMV7_CLIDR_CTYPE_INSTRUCTION_DATA) ||
                    (cache_type == ARMV7_CLIDR_CTYPE_UNIFIED))
                        v7_maint_dcache_level_setway(level, operation);
                level_start_bit += 3;
        }
}

void cache_disable(uint32_t cache_bit)
{
        uint32_t reg;

        reg = get_cr();
        cp_delay();

        if (cache_bit == CR_C) {
                /* if cache isn;t enabled no need to disable */
                if ((reg & CR_C) != CR_C)
                        return;
                /* if disabling data cache, disable mmu too */
                cache_bit |= CR_M;
        }
        reg = get_cr();
        cp_delay();
        if (cache_bit == (CR_C | CR_M))
                flush_dcache_all();
        set_cr(reg & ~cache_bit);
}

