#define PRM_RSTCTRL 0x44E00F00

#define PRM_RSTCTRL_WARM_RESET 0x01
#define PRM_RSTCTRL_COLD_RESET 0x02

#define WARM_RESET 1
#define COLD_RESET 2

void reset_cpu(int);
