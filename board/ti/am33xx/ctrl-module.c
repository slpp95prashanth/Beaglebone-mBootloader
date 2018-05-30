#include <asm/types.h>
#include <asm/io.h>
#include "ctrl-module.h"

void _config_ctrl_module(uint32_t mode, uint32_t addr)
{
    writel(mode, addr);
}

void config_ctrl_module(void)
{
/* SERIAL_UART */
//#ifdef SERIAL_UART
    _config_ctrl_module(MODE(0) | PULLUP_EN | RXACTIVE, AM335X_CTRL_CONF_UART0_RXD);
    _config_ctrl_module(MODE(0) | PULLUDEN, AM335X_CTRL_CONF_UART0_TXD);
//#endif
/* SERIAL_UART */
    return;
}

