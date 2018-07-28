#include "ddr.h"
#include <asm/io.h>

#define CM_PER                          0x44E00000
#define CM_WKUP                         0x44E00400
#define CM_DPLL                         0x44E00500

#ifdef DDR

const struct cm_perpll *cmper = (struct cm_perpll *)CM_PER;
const struct cm_wkuppll *cmwkup = (struct cm_wkuppll *)CM_WKUP;
const struct cm_dpll *cmdpll = (struct cm_dpll *)CM_DPLL;

#define PRCM_EMIF_CLK_ACTIVITY  BIT(2)
#define PRCM_L3_GCLK_ACTIVITY   BIT(4)

#define PLL_BYPASS_MODE         0x4
#define ST_MN_BYPASS            0x00000100
#define ST_DPLL_CLK             0x00000001
#define CLK_SEL_MASK            0x7ffff
#define CLK_DIV_MASK            0x1f
#define CLK_DIV2_MASK           0x7f
#define CLK_SEL_SHIFT           0x8
#define CLK_MODE_SEL            0x7
#define CLK_MODE_MASK           0xfffffff8
#define CLK_DIV_SEL             0xFFFFFFE0
#define CPGMAC0_IDLE            0x30000

#define V_OSCK                          24000000  /* Clock output from T2 */
#define OSC     (V_OSCK/1000000)

#define DDRPLL_M        266
#define DDRPLL_N        (OSC-1)
#define DDRPLL_M2       1

/* DDR Base address */
#define DDR_CTRL_ADDR                   0x44E10E04
#define DDR_CONTROL_BASE_ADDR           0x44E11404
#define DDR_PHY_BASE_ADDR               0x44E12000
#define DDR_PHY_BASE_ADDR2              0x44E120A4

void ddr_pll_config(unsigned int ddrpll_m)
{
        u32 clkmode, clksel, div_m2;

        clkmode = readl(&cmwkup->clkmoddpllddr);
        clksel = readl(&cmwkup->clkseldpllddr);
        div_m2 = readl(&cmwkup->divm2dpllddr);

        /* Set the PLL to bypass Mode */
        clkmode = (clkmode & CLK_MODE_MASK) | PLL_BYPASS_MODE;
        writel(clkmode, &cmwkup->clkmoddpllddr);

        /* Wait till bypass mode is enabled */
        while ((readl(&cmwkup->idlestdpllddr) & ST_MN_BYPASS)
                                != ST_MN_BYPASS)
                ;

        clksel = clksel & (~CLK_SEL_MASK);
        clksel = clksel | ((ddrpll_m << CLK_SEL_SHIFT) | DDRPLL_N);
        writel(clksel, &cmwkup->clkseldpllddr);

        div_m2 = div_m2 & CLK_DIV_SEL;
        div_m2 = div_m2 | DDRPLL_M2;
        writel(div_m2, &cmwkup->divm2dpllddr);

        clkmode = (clkmode & CLK_MODE_MASK) | CLK_MODE_SEL;
        writel(clkmode, &cmwkup->clkmoddpllddr);

        /* Wait till dpll is locked */
        while ((readl(&cmwkup->idlestdpllddr) & ST_DPLL_CLK) != ST_DPLL_CLK)
                ;
}

/* VTP Registers */
struct vtp_reg {
        unsigned int vtp0ctrlreg;
};

struct ddr_ctrl {
        unsigned int ddrioctrl;
        unsigned int resv1[325];
        unsigned int ddrckectrl; 
};      

#define VTP_CTRL_READY          (0x1 << 5)
#define VTP_CTRL_ENABLE         (0x1 << 6)
#define VTP_CTRL_START_EN       (0x1)
#define PHY_DLL_LOCK_DIFF       0x0
#define DDR_CKE_CTRL_NORMAL     0x1

#define VTP0_CTRL_ADDR                  0x44E10E0C
#define DDR_CTRL_ADDR                   0x44E10E04

static struct vtp_reg *vtpreg = (struct vtp_reg *)VTP0_CTRL_ADDR;
static struct ddr_ctrl *ddrctrl = (struct ddr_ctrl *)DDR_CTRL_ADDR;

static void config_vtp(void)
{
        writel(readl(&vtpreg->vtp0ctrlreg) | VTP_CTRL_ENABLE,
                        &vtpreg->vtp0ctrlreg);
        writel(readl(&vtpreg->vtp0ctrlreg) & (~VTP_CTRL_START_EN),
                        &vtpreg->vtp0ctrlreg);
        writel(readl(&vtpreg->vtp0ctrlreg) | VTP_CTRL_START_EN,
                        &vtpreg->vtp0ctrlreg);

        /* Poll for READY */
        while ((readl(&vtpreg->vtp0ctrlreg) & VTP_CTRL_READY) !=
                        VTP_CTRL_READY)
                ;
}

static struct ddr_regs *ddr_reg[2] = {
                                (struct ddr_regs *)DDR_PHY_BASE_ADDR,
                                (struct ddr_regs *)DDR_PHY_BASE_ADDR2};


/**
 * Configure DDR CMD control registers
 */
void config_cmd_ctrl(const struct cmd_control *cmd)
{
        writel(cmd->cmd0csratio, &ddr_reg[0]->cm0csratio);
        writel(cmd->cmd0dldiff, &ddr_reg[0]->cm0dldiff);
        writel(cmd->cmd0iclkout, &ddr_reg[0]->cm0iclkout);

        writel(cmd->cmd1csratio, &ddr_reg[0]->cm1csratio);
        writel(cmd->cmd1dldiff, &ddr_reg[0]->cm1dldiff);
        writel(cmd->cmd1iclkout, &ddr_reg[0]->cm1iclkout);

        writel(cmd->cmd2csratio, &ddr_reg[0]->cm2csratio);
        writel(cmd->cmd2dldiff, &ddr_reg[0]->cm2dldiff);
        writel(cmd->cmd2iclkout, &ddr_reg[0]->cm2iclkout);
}

void config_ddr_data(int macrono, const struct ddr_data *data)
{       
        writel(data->datardsratio0, &ddr_reg[macrono]->dt0rdsratio0);
        writel(data->datawdsratio0, &ddr_reg[macrono]->dt0wdsratio0);
        writel(data->datawiratio0, &ddr_reg[macrono]->dt0wiratio0);
        writel(data->datagiratio0, &ddr_reg[macrono]->dt0giratio0);
        writel(data->datafwsratio0, &ddr_reg[macrono]->dt0fwsratio0);
        writel(data->datawrsratio0, &ddr_reg[macrono]->dt0wrsratio0);
        writel(data->datauserank0delay, &ddr_reg[macrono]->dt0rdelays0);
        writel(data->datadldiff0, &ddr_reg[macrono]->dt0dldiff0);
}       

/**
 * Base address for ddr io control instances
 */
static struct ddr_cmdtctrl *ioctrl_reg = {
                        (struct ddr_cmdtctrl *)DDR_CONTROL_BASE_ADDR};

void config_io_ctrl(unsigned long val)
{
        writel(val, &ioctrl_reg->cm0ioctl);
        writel(val, &ioctrl_reg->cm1ioctl);
        writel(val, &ioctrl_reg->cm2ioctl);
        writel(val, &ioctrl_reg->dt0ioctl);
        writel(val, &ioctrl_reg->dt1ioctl);
}

/* EMIF Base address */
#define EMIF4_0_CFG_BASE                0x4C000000
#define EMIF4_1_CFG_BASE                0x4D000000

static struct emif_reg_struct *emif_reg = {
                                (struct emif_reg_struct *)EMIF4_0_CFG_BASE};


void config_ddr_phy(const struct emif_regs *regs)
{
        writel(regs->emif_ddr_phy_ctlr_1, &emif_reg->emif_ddr_phy_ctrl_1);
        writel(regs->emif_ddr_phy_ctlr_1, &emif_reg->emif_ddr_phy_ctrl_1_shdw);
}                       

/**     
 * Set SDRAM timings    
 */
void set_sdram_timings(const struct emif_regs *regs)
{
        writel(regs->sdram_tim1, &emif_reg->emif_sdram_tim_1);
        writel(regs->sdram_tim1, &emif_reg->emif_sdram_tim_1_shdw);
        writel(regs->sdram_tim2, &emif_reg->emif_sdram_tim_2);
        writel(regs->sdram_tim2, &emif_reg->emif_sdram_tim_2_shdw);
        writel(regs->sdram_tim3, &emif_reg->emif_sdram_tim_3);
        writel(regs->sdram_tim3, &emif_reg->emif_sdram_tim_3_shdw);
}

#define CTRL_BASE                       0x44E10000

/* Control Status Register */
struct ctrl_stat {
        unsigned int resv1[16];
        unsigned int statusreg;         /* ofset 0x40 */
        unsigned int resv2[51];
        unsigned int secure_emif_sdram_config;  /* offset 0x0110 */
};      

struct ctrl_stat *cstat = (struct ctrl_stat *)CTRL_BASE;

void config_sdram(const struct emif_regs *regs)
{
        writel(regs->ref_ctrl, &emif_reg->emif_sdram_ref_ctrl);
        writel(regs->ref_ctrl, &emif_reg->emif_sdram_ref_ctrl_shdw);
        if (regs->zq_config){
                writel(regs->zq_config, &emif_reg->emif_zq_config);
                writel(regs->sdram_config, &cstat->secure_emif_sdram_config);
        }
        writel(regs->sdram_config, &emif_reg->emif_sdram_config);
}

void config_ddr(unsigned int pll, unsigned int ioctrl,
                const struct ddr_data *data, const struct cmd_control *ctrl,
                const struct emif_regs *regs)
{
        enable_emif_clocks();
        ddr_pll_config(pll);
        config_vtp();
        config_cmd_ctrl(ctrl);

        config_ddr_data(0, data);
        config_ddr_data(1, data);

        config_io_ctrl(ioctrl);

        /* Set CKE to be controlled by EMIF/DDR PHY */
        writel(DDR_CKE_CTRL_NORMAL, &ddrctrl->ddrckectrl);

        /* Program EMIF instance */
        config_ddr_phy(regs);
        set_sdram_timings(regs);
        config_sdram(regs);
}

#endif /* DDR */
