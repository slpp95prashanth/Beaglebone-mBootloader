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

/* CPSW_SS */
#define CPSW_SS_SOFT_RESET	(CPSW_SS + 0x8)

/* CPSW_CPDMA */
#define CPSW_CPDMA_SOFT_RESET	(CPSW_CPDMA + 0x1c)

/* CPSW_ALE */
#define CPSW_ALE_CONTROL	(CPSW_ALE + 0x08)
#define CPSW_ALE_PORTCTL0	(CPSW_ALE + 0x40)

/* CPSW_ALE CONTROL*/
#define CPSW_ALE_CONTROL_ALE_ENABLE	(31)
#define CPSW_ALE_CONTROL_ALE_TABLE_CLEAR	(30)

/* CPSW_ALE_PORTCTL0 */
#define CPSW_ALE_PORTCTL0_PORT_STATE_FORWARD	(3)

/* CPSW_PORT */
#define CPSW_PORT_P0_CPDMA_TX_PRI_MAP	(CPSW_PORT + 0x1c)
#define CPSW_PORT_P0_CPDMA_RX_CH_MAP	(CPSW_PORT + 0x20)

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

//	writel(0x76543210, CPSW_PORT_P0_CPDMA_TX_PRI_MAP);
//	writel(0x0, CPSW_PORT_P0_CPDMA_RX_CH_MAP);
//	writel(0x0, 0x4a100010);
	writel(1 << CPSW_MDIO_STAT_PORT_EN_P0_STAT_EN, CPSW_MDIO_STAT_PORT_EN);
	writel(CPSW_ALE_PORTCTL0_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL0);

	/* MAC SOFTRESET */
	tfp_printf("Resetting MAC ...\n");
	set_and_wait_to_clear(0, CPSW_SL1_SOFT_RESET);

//	writel(0x76543210, 0x4a100da4);
//	writel(0x33221100, 0x4a100218);

	/* MAC RX_MAXLEN set to 0x5ee */
	writel(0x5ee, CPSW_SL1_RX_MAXLEN);

	writel(0x94ff3500, 0x4a100224);
	writel(0x6785, 0x4a100220);


	writel(0x3, 0x4a100d44);

	writel(0x8021, 0x4a100d84);
	writel(0x1, 0x4a10081c);
	printf("%d\n", __LINE__);
	sleep();

	writel(0x0, 0x4a100a20);
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

	writel(1, CPSW_CPDMA + 0x04);
	writel(1, CPSW_CPDMA + 0x14);

	tfp_printf("DMASTATUS = %08x\n", readl(CPSW_CPDMA + 0x24));
	writel(0, CPSW_CPDMA + 0x24);

	int data, temp;

	for (i = 0; i < pkt_len; i = i + 4) {
		temp = readl(0x80000000 + i);
		data = (temp << 24) | ((temp & 0x0000ff00) << 8) | ((temp & 0x00ff0000) >> 8) | ((temp & 0xff000000) >> 24);
		tfp_printf("%08x ", data);
	}

	rx_desc->bufoff_len = 2000;
	rx_desc->flags_pktlen = 0x20000000 | 2000;
	writel((unsigned int)rx_desc, CPSW_STATERAM + 0x20);

#if 0
	/* soft reset */
	writel(1, CPSW_WR + 0x4);
	/* ale enable */
	tmp = readl(CPSW_ALE + 0x8);
	tmp |= 0x80000000;
	writel(tmp, CPSW_ALE + 0x8);
	
	/* ale clear */
	tmp = readl(CPSW_ALE + 0x8);
	tmp |= 0x40000000;
	writel (tmp, CPSW_ALE + 0x8);

	/* vlan unaware */
	tmp = readl(CPSW_ALE + 0x8);
	tmp |= 0x4;
	writel(tmp, CPSW_ALE + 0x8);

	/* cpdma tx priority mapping */
	writel(0x76543210, CPSW_PORT + 0x1c);
	/* cpdma rx channel mapping */
	writel(0x0, CPSW_PORT + 0x20);

	/* disable priority elevation */
	writel(0x0, CPSW_SS + 0x10);

	/* enable port statistics */
	writel(0x1, CPSW_SS + 0xc);

	/* set ale to forward mode */
	tmp = readl(CPSW_ALE + 0x40);
	tmp |= 0x3;
	writel(tmp, CPSW_ALE + 0x40);

	/*
	rx_desc[1].next = NULL;
	rx_desc[1].bufptr = (uint32_t)0x80008000;
	rx_desc[1].bufoff_len = 2000;
	rx_desc[1].flags_pktlen = 0x20000000;

	writel(&rx_desc[0], CPSW_STATERAM + 0x20);
	writel(&rx_desc[1], CPSW_STATERAM + 0x24);

	writel(0x1, CPSW_CPDMA + 0x14);

	while (1) {
		tfp_printf("Waiting for packet\n");

		while ((rx_desc[0].flags_pktlen & 0x20000000) == 0x20000000);

		int len = rx_desc[0].flags_pktlen & 0x000007ff;

		tfp_printf("%d len\n", len);
	}
*/
#endif
}

	
