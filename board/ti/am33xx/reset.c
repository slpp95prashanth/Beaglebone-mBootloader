#include<asm/io.h>
#include<asm/prcm.h>

void reset_cpu(int type)
{
    writel(type == COLD_RESET ? PRM_RSTCTRL_COLD_RESET : PRM_RSTCTRL_WARM_RESET, PRM_RSTCTRL);
}

