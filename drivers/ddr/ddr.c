#include "ddr.h"
#include <asm/io.h>

static const struct ddr_data ddr3_data = {
        .datardsratio0 = MT41J128MJT125_RD_DQS,
        .datawdsratio0 = MT41J128MJT125_WR_DQS,
        .datafwsratio0 = MT41J128MJT125_PHY_FIFO_WE,
        .datawrsratio0 = MT41J128MJT125_PHY_WR_DATA,
        .datadldiff0 = PHY_DLL_LOCK_DIFF,
};

static const struct cmd_control ddr3_cmd_ctrl_data = {
        .cmd0csratio = MT41J128MJT125_RATIO,
        .cmd0dldiff = MT41J128MJT125_DLL_LOCK_DIFF,
        .cmd0iclkout = MT41J128MJT125_INVERT_CLKOUT,

        .cmd1csratio = MT41J128MJT125_RATIO,
        .cmd1dldiff = MT41J128MJT125_DLL_LOCK_DIFF,
        .cmd1iclkout = MT41J128MJT125_INVERT_CLKOUT,

        .cmd2csratio = MT41J128MJT125_RATIO,
        .cmd2dldiff = MT41J128MJT125_DLL_LOCK_DIFF,
        .cmd2iclkout = MT41J128MJT125_INVERT_CLKOUT,
};

static struct emif_regs ddr3_emif_reg_data = {
        .sdram_config = MT41J128MJT125_EMIF_SDCFG,
        .ref_ctrl = MT41J128MJT125_EMIF_SDREF,
        .sdram_tim1 = MT41J128MJT125_EMIF_TIM1,
        .sdram_tim2 = MT41J128MJT125_EMIF_TIM2,
        .sdram_tim3 = MT41J128MJT125_EMIF_TIM3,
        .zq_config = MT41J128MJT125_ZQ_CFG,
        .emif_ddr_phy_ctlr_1 = MT41J128MJT125_EMIF_READ_LATENCY,
};

void ddr_init(void)
{
    gpio_direction_out(7);
    config_ddr(303, MT41J128MJT125_IOCTRL_VALUE, &ddr3_data, 
		    &ddr3_cmd_ctrl_data, &ddr3_emif_reg_data);

    return ;
}
