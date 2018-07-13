#ifndef _AM335X_GPIO_H

#define BANK_SIZE   32
#define MAX_BANK    3

#define AM335x_GPIO0_BASE   0x44e07000
#define AM335x_GPIO1_BASE   0x4804c000

#define AM335X_GPIO_IRQSTATUS_0	0x2c
#define AM335X_GPIO_IRQSTATUS_SET_0 0x34
#define AM335X_GPIO_OE 0x134
#define AM335X_GPIO_DATAIN 0x138
#define AM335X_GPIO_LEVELDETECT0    0X140
#define AM335X_GPIO_LEVELDETECT1    0X144
#define AM335X_GPIO_RISINGDETECT    0x148
#define AM335X_GPIO_FALLINGDETECT   0x14c
#define AM335X_GPIO_DATACLEAR	0x190
#define AM335X_GPIO_DATASET 0x194

#define OUT 0
#define IN  1

#endif /* _AM335X_GPIO_H */
