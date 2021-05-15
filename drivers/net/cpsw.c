#include <stdio.h>
#include <lib/io.h>
#include <asm/io.h>
#include <asm/types.h>
#include <net/cpsw.h>
#include <net/cpsw-stats.h>

#define MAC_ADDR_LO	(0x6785)
#define MAC_ADDR_HI	(0x94ff3500)

struct desc {
	volatile struct desc *next;
	volatile uint32_t bufptr;
	volatile uint32_t bufoff_len;
	volatile uint32_t flags_pktlen;
};

struct cpsw_priv {
	struct desc *rx_desc;
	char mac[6];
};

static struct cpsw_priv priv;

void cpsw_configure_ctrl_module(void)
{
	printf("Configuring Control Module\n");
	eth_init();
}

void cpsw_mdio_init(void)
{
	/* MDIO CLK DIV set to 0xff */
	writel((1 << CPSW_MDIO_CONTROL_ENABLE_BIT) | 0xff, CPSW_MDIO_CONTROL);
}

void cpsw_ale_init(void)
{
	printf("COnfiguring ALE\n");
	__read_and_write(1, CPSW_ALE_CONTROL_ALE_ENABLE_BIT, CPSW_ALE_CONTROL);
	__read_and_write(1, CPSW_ALE_CONTROL_ALE_TABLE_CLEAR_BIT, CPSW_ALE_CONTROL);

	/* Enable PORT Statistics */
	writel((1 << CPSW_SS_STAT_PORT_EN_P0_STAT_EN_BIT) | (1 << CPSW_SS_STAT_PORT_EN_P1_STAT_EN_BIT) | (1 << CPSW_SS_STAT_PORT_EN_P2_STAT_EN_BIT), CPSW_SS_STAT_PORT_EN);
	/* Set PORT 0 to Forward mode */
	writel(CPSW_ALE_PORTCTL_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL0);
	/* Set PORT 1 to Forward Mode */
	writel(CPSW_ALE_PORTCTL_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL1);

}

void cpsw_set_mac(void)
{
	int mac_lo, mac_hi;

	writel(MAC_ADDR_HI, CPSW_PORT_P1_SA_HI);
	writel(MAC_ADDR_LO, CPSW_PORT_P1_SA_LO);

	mac_lo = readl(CPSW_PORT_P1_SA_LO);
	mac_hi = readl(CPSW_PORT_P1_SA_HI);

	printf("MAC addr = %02x:%02x:%02x:%02x:%02x:%02x\n", mac_hi & 0xff, (mac_hi >> 8) & 0xff, (mac_hi >> 16) & 0xff, (mac_hi >> 24) & 0xff, (mac_lo) & 0xff, (mac_lo >> 8) & 0xff);
}

void cpsw_ss_softreset(void)
{
	printf("Resetting CPSW_SS ...\n");
	set_and_wait_to_clear(0, CPSW_SS_SOFT_RESET);
}

void cpsw_cpdma_softreset(void)
{
	printf("Resetting CPSW_CPDMA ...\n");
	set_and_wait_to_clear(0, CPSW_CPDMA_SOFT_RESET);
}

void cpsw_sl1_softreset(void)
{
	/* MAC SOFTRESET */
	printf("Resetting MAC ...\n");
	set_and_wait_to_clear(0, CPSW_SL1_SOFT_RESET);
}

void cpsw_write_rx_hdp(void *desc)
{
	printf("Write RX head desc pointer  = %08x\n", desc);
	writel((unsigned int)desc, CPSW_STATERAM_RX0_HDP);
}

void *rx_buffer_alloc(size_t size)
{
	return (void *)0x80000000;
}

void cpsw_rx_desc_init(struct desc *desc)
{
	struct desc *rx_desc = desc;

	rx_desc->next = NULL;
	rx_desc->bufptr = rx_buffer_alloc(CPSW_RX_MAX_PKT_LEN);
	rx_desc->bufoff_len = CPSW_RX_MAX_PKT_LEN;
	rx_desc->flags_pktlen = CPSW_DESC_OWNERSHIP_DMA | CPSW_RX_MAX_PKT_LEN;
}

void cpsw_cpdma_enable_rx(void)
{
	printf("Enabling cpdma rx\n");
	writel(1 << CPSW_CPDMA_RX_CONTROL_RX_EN_BIT, CPSW_CPDMA_RX_CONTROL);
}

void cpsw_cpdma_enable_tx(void)
{
	printf("Enabling cpdma tx\n");
	writel(1 << CPSW_CPDMA_TX_CONTROL_TX_EN_BIT, CPSW_CPDMA_TX_CONTROL);
}

void do_eth_stats(void)
{
	cpsw_stats_rx();
}

void cpsw_recv(void)
{
	struct desc *rx_desc;
	int i;

	rx_desc = priv.rx_desc;

	printf("Waiting for packet to receive ...\n");
	while (rx_desc->flags_pktlen & CPSW_DESC_OWNERSHIP_DMA);

	printf("Rx frames = %08x\n", readl(CPSW_STATS));
	printf("pkt_len = %08x\n", rx_desc->flags_pktlen & CPSW_DESC_PKT_LEN_MASK);

	int pkt_len = rx_desc->flags_pktlen & CPSW_DESC_PKT_LEN_MASK;

	printf("DMASTATUS = %08x\n", readl(CPSW_CPDMA_DMASTATUS));

	int data, temp;

	for (i = 0; i < pkt_len; i = i + 4) {
		temp = readl(rx_desc->bufptr + i);
		data = (temp << 24) | ((temp & 0x0000ff00) << 8) | ((temp & 0x00ff0000) >> 8) | ((temp & 0xff000000) >> 24);
		printf("%08x ", data);
	}

	rx_desc->bufoff_len = CPSW_RX_MAX_PKT_LEN;
	rx_desc->flags_pktlen = CPSW_DESC_OWNERSHIP_DMA | CPSW_RX_MAX_PKT_LEN;

	cpsw_write_rx_hdp(rx_desc);
}

int cpsw_init(void)
{
	priv.rx_desc = (void *)CPSW_CPPI_RAM;

	cpsw_configure_ctrl_module();

	cpsw_ss_softreset();
	cpsw_cpdma_softreset();

	cpsw_mdio_init();

	cpsw_ale_init();

	/* MAC CPSW_RX_MAXLEN set to 0x5ee */
	writel(CPSW_RX_MAX_PKT_LEN, CPSW_SL1_RX_MAXLEN);

	cpsw_set_mac();

	writel((1 << CPSW_SL1_MACCONTROL_IFCTL_A_BIT) | (1 << CPSW_SL1_MACCONTROL_GMII_EN_BIT) | (1 << CPSW_SL1_MACCONTROL_FULLDUPLEX_BIT), CPSW_SL1_MACCONTROL);

	cpsw_cpdma_softreset();

	cpsw_rx_desc_init(priv.rx_desc);

	/* Write RX desc pointer */
	cpsw_write_rx_hdp(priv.rx_desc);

	cpsw_cpdma_enable_tx();
	cpsw_cpdma_enable_rx();
}

