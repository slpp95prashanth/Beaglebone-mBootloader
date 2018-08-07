#include <asm/types.h>
#include <asm/io.h>
#include "../board/ti/am33xx/clock.h"
#include "../board/ti/am33xx/ctrl-module.h"

extern int M_BOOT_END_ADDR;

void *malloc(int size)
{
    static int malloc_free_block = MALLOC_ADDR;

    malloc_free_block += size;

    return malloc_free_block - size;
}

void _config_ctrl_module(uint32_t mode, uint32_t addr)
{
    writel(mode, addr);
}

void relocate_code(void)
{
    int ret, d_len;

    ret = lzma_inflate(&M_BOOT_END_ADDR, LZMA_SIZE, LOADADDR_DDR, &d_len, &malloc);
    if (ret) {
	while (1) {
	    NS16550_putc('E');
	    NS16550_putc('\n');
	}
    }

    return ;
}

void *memcpy (void *dest, const void *src, unsigned int len)
{
    char *d = dest;
    const char *s = src;

    while (len--)
	*d++ = *s++;

    return dest;
}

void enable_emif_clocks(void)
{
        /* Enable the  EMIF_FW Functional clock */
        writel(PRCM_MOD_EN, AM335X_CM_PER_EMIF_FW_CLKCTRL);
        /* Enable EMIF0 Clock */
        writel(PRCM_MOD_EN, AM335X_CM_PER_EMIF_CLKCTRL);
        /* Poll if module is functional */
        while ((readl(AM335X_CM_PER_EMIF_CLKCTRL)) != PRCM_MOD_EN)
                ;
}

void early_system_init(void)
{
    void (*go)(void) = ENTRY_DDR;

    _config_ctrl_module(0 | PULLUP_EN | RXACTIVE, AM335X_CTRL_CONF_UART0_RXD);
    _config_ctrl_module(0 | PULLUDEN, AM335X_CTRL_CONF_UART0_TXD);

    writel(PRCM_MOD_EN, AM335X_CLK_WKUP_UART0);
    while (readl(AM335X_CLK_WKUP_UART0) != PRCM_MOD_EN)
           ;

    enable_emif_clocks();

    uart_console_init();

    ddr_init();

    relocate_code();

    go();
}

void board_init_f(ulong dummy)
{
    early_system_init();
    return;
}
