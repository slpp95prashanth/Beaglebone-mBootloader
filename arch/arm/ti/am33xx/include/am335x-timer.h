#ifndef _AM335X_TIMER

#define AM335X_DMTIMER0	0x44e05000
#define AM335X_DMTIMER1_1MS	0x44E31000

/* AM335X_DMTIMER0 */
#define AM335X_DMTIMER0_IRQSTATUS	(AM335X_DMTIMER0 + 0x28)
#define AM335X_DMTIMER0_IRQENABLE_SET	(AM335X_DMTIMER0 + 0x2c)
#define AM335X_DMTIMER0_TLCR	(AM335X_DMTIMER0 + 0x38)
#define AM335X_DMTIMER0_TCRR	(AM335X_DMTIMER0 + 0x3c)
#define AM335X_DMTIMER0_TLDR	(AM335X_DMTIMER0 + 0x40)

#define AM335X_DMTIMER0_CLK   32000

/* AM335X_DMTIMER1_1MS */
#define AM335X_DMTIMER1_1MS_IRQSTATUS	(AM335X_DMTIMER1_1MS + 0x18)
#define AM335X_DMTIMER1_1MS_IRQENABLE_SET	(AM335X_DMTIMER1_1MS + 0x1c)
#define AM335X_DMTIMER1_1MS_TLCR	(AM335X_DMTIMER1_1MS + 0x24)
#define AM335X_DMTIMER1_1MS_TCRR	(AM335X_DMTIMER1_1MS + 0x28)
#define AM335X_DMTIMER1_1MS_TLDR	(AM335X_DMTIMER1_1MS + 0x2c)

#define AM335X_DMTIMER1_1MS_CLK   32000

#define OVF_EN_FLAG 0x2
#define START_TIMER 0x1
#define AUTO_RELOAD 0x2
#define TRIGGER_ON_OVERFLOW 0x400

#endif /* _AM335X_TIMER */
