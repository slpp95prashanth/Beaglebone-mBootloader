#include <stdio.h>
#include <lib/io.h>
#include <asm/io.h>
#include <asm/types.h>

#define CPSW_WR 0x4A101200
#define CPSW_ALE 0x4A100D00
#define CPSW_SS 0x4A100000
#define CPSW_PORT 0x4A100100
#define CPSW_CPDMA 0x4A100800
#define CPSW_STATS 0x4A100900
#define CPSW_STATERAM 0x4A100A00
#define CPSW_CPTS 0x4A100C00
#define CPSW_SL1 0x4A100D80
#define CPSW_SL2 0x4A100DC0
#define CPSW_MDIO 0x4A101000

/* CPSW_MDIO */
#define CPSW_MDIO_CONTROL	(CPSW_MDIO + 0x4)
#define CPSW_MDIO_STAT_PORT_EN	(CPSW_MDIO + 0xc)

/* CPSW_MDIO_CONTROL */
#define CPSW_MDIO_CONTROL_ENABLE	(30)

/* CPSW_MDIO_STAT_PORT_EN */
#define CPSW_MDIO_STAT_PORT_EN_P0_STAT_EN	(0)
#define CPSW_MDIO_STAT_PORT_EN_P1_STAT_EN	(1)
#define CPSW_MDIO_STAT_PORT_EN_P2_STAT_EN	(2)

/* CPSW_SL1 */
#define CPSW_SL1_SOFT_RESET	(CPSW_SL1 + 0x0c)
#define CPSW_SL1_RX_MAXLEN	(CPSW_SL1 + 0x10)
#define CPSW_SL1_MACCONTROL	(CPSW_SL1 + 0x04)

/* CPSW_SL1_MACCONTROL */
#define CPSW_SL1_MACCONTROL_FULLDUPLEX	(0)
#define CPSW_SL1_MACCONTROL_GMII_EN	(5)
#define CPSW_SL1_MACCONTROL_IFCTL_A	(15)

/* CPSW_SS */
#define CPSW_SS_SOFT_RESET	(CPSW_SS + 0x8)

/* CPSW_CPDMA */
#define CPSW_CPDMA_SOFT_RESET	(CPSW_CPDMA + 0x1c)
#define CPSW_CPDMA_TX_CONTROL	(CPSW_CPDMA + 0x04)
#define CPSW_CPDMA_RX_CONTROL	(CPSW_CPDMA + 0x14)
#define CPSW_CPDMA_DMASTATUS	(CPSW_CPDMA + 0x24)

/* CPSW_ALE */
#define CPSW_ALE_CONTROL	(CPSW_ALE + 0x08)
#define CPSW_ALE_PORTCTL0	(CPSW_ALE + 0x40)
#define CPSW_ALE_PORTCTL1	(CPSW_ALE + 0x44)

/* CPSW_ALE CONTROL*/
#define CPSW_ALE_CONTROL_ALE_ENABLE	(31)
#define CPSW_ALE_CONTROL_ALE_TABLE_CLEAR	(30)

/* CPSW_PORT */
#define CPSW_PORT_P0_CPDMA_TX_PRI_MAP	(CPSW_PORT + 0x1c)
#define CPSW_PORT_P0_CPDMA_RX_CH_MAP	(CPSW_PORT + 0x20)
#define CPSW_PORT_P1_SA_LO		(CPSW_PORT + 0x20)
#define CPSW_PORT_P1_SA_HI		(CPSW_PORT + 0x24)

/* CPSW_ALE_PORTCTL */
#define CPSW_ALE_PORTCTL_PORT_STATE_FORWARD	(3)

/* CPSW_STATERAM */
#define CPSW_STATERAM_TX0_HDP	(CPSW_STATERAM + 0X00)
#define CPSW_STATERAM_TX1_HDP	(CPSW_STATERAM + 0X04)
#define CPSW_STATERAM_TX2_HDP	(CPSW_STATERAM + 0X08)
#define CPSW_STATERAM_TX3_HDP	(CPSW_STATERAM + 0X0c)
#define CPSW_STATERAM_TX4_HDP	(CPSW_STATERAM + 0X10)
#define CPSW_STATERAM_TX5_HDP	(CPSW_STATERAM + 0X14)
#define CPSW_STATERAM_TX6_HDP	(CPSW_STATERAM + 0X18)
#define CPSW_STATERAM_TX7_HDP	(CPSW_STATERAM + 0X1c)
#define CPSW_STATERAM_RX0_HDP	(CPSW_STATERAM + 0X20)
#define CPSW_STATERAM_RX1_HDP	(CPSW_STATERAM + 0X24)
#define CPSW_STATERAM_RX2_HDP	(CPSW_STATERAM + 0X28)
#define CPSW_STATERAM_RX3_HDP	(CPSW_STATERAM + 0X2c)
#define CPSW_STATERAM_RX4_HDP	(CPSW_STATERAM + 0X30)
#define CPSW_STATERAM_RX5_HDP	(CPSW_STATERAM + 0X34)
#define CPSW_STATERAM_RX6_HDP	(CPSW_STATERAM + 0X38)
#define CPSW_STATERAM_RX7_HDP	(CPSW_STATERAM + 0X3c)
#define CPSW_STATERAM_TX0_CP	(CPSW_STATERAM + 0X40)
#define CPSW_STATERAM_TX1_CP	(CPSW_STATERAM + 0X44)
#define CPSW_STATERAM_TX2_CP	(CPSW_STATERAM + 0X48)
#define CPSW_STATERAM_TX3_CP	(CPSW_STATERAM + 0X4c)
#define CPSW_STATERAM_TX4_CP	(CPSW_STATERAM + 0X50)
#define CPSW_STATERAM_TX5_CP	(CPSW_STATERAM + 0X54)
#define CPSW_STATERAM_TX6_CP	(CPSW_STATERAM + 0X58)
#define CPSW_STATERAM_TX7_CP	(CPSW_STATERAM + 0X5c)
#define CPSW_STATERAM_RX0_CP	(CPSW_STATERAM + 0X60)
#define CPSW_STATERAM_RX1_CP	(CPSW_STATERAM + 0X64)
#define CPSW_STATERAM_RX2_CP	(CPSW_STATERAM + 0X68)
#define CPSW_STATERAM_RX3_CP	(CPSW_STATERAM + 0X6c)
#define CPSW_STATERAM_RX4_CP	(CPSW_STATERAM + 0X70)
#define CPSW_STATERAM_RX5_CP	(CPSW_STATERAM + 0X74)
#define CPSW_STATERAM_RX6_CP	(CPSW_STATERAM + 0X78)
#define CPSW_STATERAM_RX7_CP	(CPSW_STATERAM + 0X7c)

struct desc {
	volatile struct desc *next;
	volatile uint32_t bufptr;
	volatile uint32_t bufoff_len;
	volatile uint32_t flags_pktlen;
};

void __read_and_write(unsigned short val, unsigned int bit, void *addr)
{
	int mask = 0xffffffff;

	int tmp = readl(addr);
	mask &= 1 << bit;
	tmp |= ((val << bit) & mask);
	tfp_printf("%08x = %08x\n", tmp, addr);
	writel(tmp, addr);
}

void set_and_wait_to_clear(int bit, void *addr)
{
	writel(1 << bit, addr);

	while (readl(addr) & (1 << bit));
}

void sleep(void)
{
	int i;

	for (i = 0; i < 0xffff; i++);
}

#define MAC_ADDR_LO	(0x6785)
#define MAC_ADDR_HI	(0x94ff3500)

int cpsw_init(void)
{
	struct desc *rx_desc;
	int tmp, i;

	rx_desc = (void *)0x4A102000;

	eth_init();
	writel(0, 0x44e10650);

	/* MDIO CLK DIV set to 0xff */
	writel((1 << CPSW_MDIO_CONTROL_ENABLE) | 0xff, CPSW_MDIO_CONTROL);

	tfp_printf("Resetting CPSW_SS ...\n");
	set_and_wait_to_clear(0, CPSW_SS_SOFT_RESET);

	tfp_printf("Resetting CPSW_CPDMA ...\n");
	set_and_wait_to_clear(0, CPSW_CPDMA_SOFT_RESET);

	__read_and_write(1, CPSW_ALE_CONTROL_ALE_ENABLE, CPSW_ALE_CONTROL);
	__read_and_write(1, CPSW_ALE_CONTROL_ALE_TABLE_CLEAR, CPSW_ALE_CONTROL);

	tfp_printf("ALE_CONTROL = %08x\n", readl(CPSW_ALE_CONTROL));

	writel(1 << CPSW_MDIO_STAT_PORT_EN_P0_STAT_EN, CPSW_MDIO_STAT_PORT_EN);
	writel(CPSW_ALE_PORTCTL_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL0);

	/* MAC SOFTRESET */
	tfp_printf("Resetting MAC ...\n");
	set_and_wait_to_clear(0, CPSW_SL1_SOFT_RESET);

	/* MAC RX_MAXLEN set to 0x5ee */
	writel(0x5ee, CPSW_SL1_RX_MAXLEN);

	writel(MAC_ADDR_HI, CPSW_PORT_P1_SA_HI);
	writel(MAC_ADDR_LO, CPSW_PORT_P1_SA_LO);

	int mac_lo, mac_hi;

	mac_lo = readl(CPSW_PORT_P1_SA_LO);
	mac_hi = readl(CPSW_PORT_P1_SA_HI);

	tfp_printf("Setting MAC addr = %02x:%02x:%02x:%02x:%02x:%02x\n", mac_hi & 0xff, (mac_hi >> 8) & 0xff, (mac_hi >> 16) & 0xff, (mac_hi >> 24) & 0xff, (mac_lo) & 0xff, (mac_lo >> 8) & 0xff);

	writel(CPSW_ALE_PORTCTL_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL1);

	writel((1 << CPSW_SL1_MACCONTROL_IFCTL_A) | (1 << CPSW_SL1_MACCONTROL_GMII_EN) | (1 << CPSW_SL1_MACCONTROL_FULLDUPLEX), CPSW_SL1_MACCONTROL);

	tfp_printf("Resetting CPSW_CPDMA ...\n");
	set_and_wait_to_clear(0, CPSW_CPDMA_SOFT_RESET);
/*
	writel(0x0, CPSW_STATERAM_RX0_HDP);
	writel(0x0, 0x4a1008e0);
	writel(0x0, 0x4a100a60);
	writel(0x0, 0x4a100a00);
	writel(0x0, 0x4a100a40);
	writel(0x0, 0x4a100a24);
	writel(0x0, 0x4a1008e4);
	writel(0x0, 0x4a100a64);
	writel(0x0, 0x4a100a04);
	writel(0x0, 0x4a100a44);
	writel(0x0, 0x4a100a28);
	writel(0x0, 0x4a1008e8);
	writel(0x0, 0x4a100a68);
	writel(0x0, 0x4a100a08);
	writel(0x0, 0x4a100a48);
	writel(0x0, 0x4a100a2c);
	writel(0x0, 0x4a1008ec);
	writel(0x0, 0x4a100a6c);
	writel(0x0, 0x4a100a0c);
	writel(0x0, 0x4a100a4c);
	writel(0x0, 0x4a100a30);
	writel(0x0, 0x4a1008f0);
	writel(0x0, 0x4a100a70);
	writel(0x0, 0x4a100a10);
	writel(0x0, 0x4a100a50);
	writel(0x0, 0x4a100a34);
	writel(0x0, 0x4a1008f4);
	writel(0x0, 0x4a100a74);
	writel(0x0, 0x4a100a14);
	writel(0x0, 0x4a100a54);
	writel(0x0, 0x4a100a38);
	writel(0x0, 0x4a1008f8);
	writel(0x0, 0x4a100a78);
	writel(0x0, 0x4a100a18);
	writel(0x0, 0x4a100a58);
	writel(0x0, 0x4a100a3c);
	writel(0x0, 0x4a1008fc);
	writel(0x0, 0x4a100a7c);
	writel(0x0, 0x4a100a1c);
	writel(0x0, 0x4a100a5c);
*/
	writel(0x1, 0x4a100804);
	writel(0x1, 0x4a100814);

	int count = 0, prev_count = 0;

	rx_desc->next = NULL;
	rx_desc->bufptr = (uint32_t)0x80000000;
	rx_desc->bufoff_len = 2000;
	rx_desc->flags_pktlen = 0x20000000 | 2000;

	writel((unsigned int)rx_desc, CPSW_STATERAM + 0x20);

	while (rx_desc->flags_pktlen & 0x20000000);

	tfp_printf("%08x = %08x\n", &(rx_desc->flags_pktlen), rx_desc->flags_pktlen);
	tfp_printf("Rx frames = %08x\n", readl(CPSW_STATS));
	tfp_printf("pkt_len = %08x\n", rx_desc->flags_pktlen & 0x7ff);

	int pkt_len = rx_desc->flags_pktlen & 0x7ff;

	writel(1, CPSW_CPDMA_TX_CONTROL);
	writel(1, CPSW_CPDMA_RX_CONTROL);

	tfp_printf("DMASTATUS = %08x\n", readl(CPSW_CPDMA_DMASTATUS));

	int data, temp;

	for (i = 0; i < pkt_len; i = i + 4) {
		temp = readl(0x80000000 + i);
		data = (temp << 24) | ((temp & 0x0000ff00) << 8) | ((temp & 0x00ff0000) >> 8) | ((temp & 0xff000000) >> 24);
		tfp_printf("%08x ", data);
	}

	rx_desc->bufoff_len = 2000;
	rx_desc->flags_pktlen = 0x20000000 | 2000;
	writel((unsigned int)rx_desc, CPSW_STATERAM + 0x20);
}

	
