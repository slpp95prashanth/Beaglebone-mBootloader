#define AM335X_CTRL_BASE 0x44e10000

#define AM335X_CTRL_CONF_UART0_RXD	(AM335X_CTRL_BASE + 0x970)
#define AM335X_CTRL_CONF_UART0_TXD	(AM335X_CTRL_BASE + 0x974)

#define SLEWCTRL        (0x1 << 6)
#define RXACTIVE        (0x1 << 5)
#define PULLUP_EN       (0x1 << 4) /* Pull UP Selection */
#define PULLUDEN        (0x0 << 3) /* Pull up enabled */
#define PULLUDDIS       (0x1 << 3) /* Pull up disabled */
#define MODE(val)       val     /* used for Readability */



 
