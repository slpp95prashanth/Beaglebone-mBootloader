#include <stdio.h>
#include <lib/io.h>
#include <lib/irq.h>
#include <lib/string.h>
#include <asm/io.h>
#include <asm/types.h>
#include <net/cpsw.h>
#include <net/cpsw-stats.h>
#include <net/debug.h>
#include <net/net.h>

#define MAC_ADDR_LO	(0x6785)
#define MAC_ADDR_HI	(0x94ff3500)

#ifdef NET

static void cpsw_configure_ctrl_module(void)
{
	printf("Configuring Control Module\n");
	eth_init();
}

static void cpsw_ss_softreset(void)
{
	printf("Resetting CPSW_SS ...\n");
	set_and_wait_to_clear(0, CPSW_SS_SOFT_RESET);
}

static void cpsw_cpdma_softreset(void)
{
	printf("Resetting CPSW_CPDMA ...\n");
	set_and_wait_to_clear(0, CPSW_CPDMA_SOFT_RESET);
}

static void cpsw_sl1_softreset(void)
{
	/* MAC SOFTRESET */
	printf("Resetting MAC ...\n");
	set_and_wait_to_clear(0, CPSW_SL1_SOFT_RESET);
}

static void cpsw_mdio_init(void)
{
	/* MDIO CLK DIV set to 0xff */
	writel((1 << CPSW_MDIO_CONTROL_ENABLE_BIT) | 0xff, CPSW_MDIO_CONTROL);
}

static void cpsw_ale_init(void)
{
	printf("COnfiguring ALE\n");

	__read_and_write(1, CPSW_ALE_CONTROL_ALE_ENABLE_BIT, CPSW_ALE_CONTROL);
	__read_and_write(1, CPSW_ALE_CONTROL_ALE_TABLE_CLEAR_BIT, CPSW_ALE_CONTROL);

	/* Flood unknown unicast packets to host port */
	__read_and_write(1, CPSW_ALE_CONTROL_EN_P0_UNI_FLOOD_BIT, CPSW_ALE_CONTROL);

	/* Enable PORT Statistics */
	writel((1 << CPSW_SS_STAT_PORT_EN_P0_STAT_EN_BIT) | (1 << CPSW_SS_STAT_PORT_EN_P1_STAT_EN_BIT) | (1 << CPSW_SS_STAT_PORT_EN_P2_STAT_EN_BIT), CPSW_SS_STAT_PORT_EN);
	/* Set PORT 0 to Forward mode */
	writel(CPSW_ALE_PORTCTL_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL0);
	/* Set PORT 1 to Forward Mode */
	writel(CPSW_ALE_PORTCTL_PORT_STATE_FORWARD, CPSW_ALE_PORTCTL1);

}

static void cpsw_set_mac(void)
{
	int mac_lo, mac_hi;

	writel(MAC_ADDR_HI, CPSW_PORT_P1_SA_HI);
	writel(MAC_ADDR_LO, CPSW_PORT_P1_SA_LO);

	mac_lo = readl(CPSW_PORT_P1_SA_LO);
	mac_hi = readl(CPSW_PORT_P1_SA_HI);

	printf("MAC addr = %02x:%02x:%02x:%02x:%02x:%02x\n", mac_hi & 0xff, (mac_hi >> 8) & 0xff, (mac_hi >> 16) & 0xff, (mac_hi >> 24) & 0xff, (mac_lo) & 0xff, (mac_lo >> 8) & 0xff);
}

static void cpsw_write_rx_hdp(void *desc)
{
	printf("Write RX head desc pointer  = %08x\n", desc);
	writel((unsigned int)desc, CPSW_STATERAM_RX0_HDP);
}

static void cpsw_write_tx_hdp(void *desc)
{
	printf("Write TX head desc pointer  = %08x\n", desc);
	writel((unsigned int)desc, CPSW_STATERAM_TX0_HDP);
}

static void *rx_buffer_alloc(size_t size)
{
	return (void *)0x80000000;
}

static void cpsw_rx_desc_init(struct desc *desc)
{
	struct desc *rx_desc = desc;

	rx_desc->next = NULL;
	rx_desc->bufptr = (uint32_t)rx_buffer_alloc(CPSW_RX_MAX_PKT_LEN);
	rx_desc->bufoff_len = CPSW_RX_MAX_PKT_LEN;
	rx_desc->flags_pktlen = CPSW_DESC_OWNERSHIP_DMA | CPSW_RX_MAX_PKT_LEN;
}

static void cpsw_tx_desc_init(struct desc *desc)
{
	struct desc *tx_desc = desc;
	int i;

	int data[] = {0xffffffff, 0xffff0059, 0x4d6ef719, 0x08060001, 0x08000604, 0x00010059, 0x4d6ef719, 0xc0a80102, 0x00000000, 0x0000c0a8, 0x01015555, 0x55555555, 0x55555555, 0x55555555, 0x55555555, 0xa40bc0ff};

	for (i = 0; i < 0x10; i++) {
		writel(htonl(data[i]), 0x80008000 + i * 4);
		printf("0x%08x ", data[i]);
	}

	printf("data \n");
	for (i = 0; i < 0x10; i++) {
		printf("0x%08x ", readl(0x80008000 + i * 4));
	}
	printf("\n");

	tx_desc->next = NULL;
	tx_desc->bufptr = 0x80008000;
	tx_desc->bufoff_len = 0x40;
	tx_desc->flags_pktlen = CPSW_DESC_SOP | CPSW_DESC_EOP | CPSW_DESC_OWNERSHIP_DMA | 0x40 | 1 << 20 | 1 << 16;

	cpsw_write_tx_hdp((void *)CPSW_CPPI_RAM + sizeof(struct desc) * 12);
}

static void cpsw_cpdma_enable_rx(void)
{
	printf("Enabling cpdma rx\n");
	writel(1 << CPSW_CPDMA_RX_CONTROL_RX_EN_BIT, CPSW_CPDMA_RX_CONTROL);
}

static void cpsw_cpdma_enable_tx(void)
{
	printf("Enabling cpdma tx\n");
	writel(1 << CPSW_CPDMA_TX_CONTROL_TX_EN_BIT, CPSW_CPDMA_TX_CONTROL);
}

void do_eth_stats(void)
{
	cpsw_stats_rx();
	cpsw_stats_tx();
}

static void cpsw_check_dma_status(void)
{
	int cpsw_dma_status = readl(CPSW_CPDMA_DMASTATUS);

	printf("DMASTATUS = %08x\n", cpsw_dma_status);

	if (cpsw_dma_status & CPSW_CPDMA_DMASTATUS_IDLE) {
		printf("DMA Idle\n");
		return ;
	}

	if ((cpsw_dma_status & 0xf000) != 0) {
		switch(cpsw_dma_status & 0xf000) {
			case 0x0010:
				printf(CPSW_CPDMA_RX_HOST_ERR_0010);
				break;
			case 0x0100:
				printf(CPSW_CPDMA_RX_HOST_ERR_0100);
				break;
			case 0x0101:
				printf(CPSW_CPDMA_RX_HOST_ERR_0101);
				break;
			case 0x0110:
				printf(CPSW_CPDMA_RX_HOST_ERR_0110);
				break;
			default:
				printf("Unknown state in CPSW_CPDMA_DMASTATUS_RX_HOST_ERR\n");
		}
		return ;
	}
}

void cpsw_send(char *buf, uint32_t len)
{
	struct desc *tx_desc = (struct desc *)((void *)CPSW_CPPI_RAM + sizeof(struct desc) * 12);

	printf("Transmitting packet\n");

	cpsw_tx_desc_init(tx_desc);
/*
	if (buf != NULL) {
		tx_desc->bufptr = buf;
		tx_desc->bufoff_len = len;
		tx_desc->flags_pktlen = (tx_desc->flags_pktlen & (~0x7ff)) | len;
	}
*/
	cpsw_cpdma_enable_tx();

	while (tx_desc->flags_pktlen & CPSW_DESC_OWNERSHIP_DMA);

	cpsw_check_dma_status();
}

static void cpsw_recv(struct cpsw_priv *priv)
{
	struct desc *rx_desc;
	int pkt_len;

	rx_desc = priv->rx_desc;

	printf("Waiting for packet to receive ...\n");
	printf("flags = %08x\n", rx_desc->flags_pktlen);

	while (rx_desc->flags_pktlen & CPSW_DESC_OWNERSHIP_DMA);

	printf("Rx frames = %08x\n", readl(CPSW_STATS));
	printf("pkt_len = %08x\n", rx_desc->flags_pktlen & CPSW_DESC_PKT_LEN_MASK);

	pkt_len = rx_desc->flags_pktlen & CPSW_DESC_PKT_LEN_MASK;

	cpsw_check_dma_status();

	print_packet((void *)rx_desc->bufptr, pkt_len);

	ethernet_input((char *)rx_desc->bufptr, pkt_len);

	memset((void *)rx_desc->bufptr, '\0', pkt_len);

	rx_desc->bufoff_len = CPSW_RX_MAX_PKT_LEN;
	rx_desc->flags_pktlen = CPSW_DESC_OWNERSHIP_DMA | CPSW_RX_MAX_PKT_LEN;

	cpsw_write_rx_hdp(rx_desc);
}

static int cpsw_irq(int irq, void *data)
{
	struct cpsw_priv *priv = data;

	writel(0, CPSW_WR_C0_RX_EN);

	if (irq == CPSW_3PGSWRXINT0) {
		printf("RX_PULSE interrupt received irq number = 0x%x\n", irq);

		cpsw_recv(priv);

		/* ACK the DMA processed descriptor by writing the descriptor address to completion pointer */
		writel(CPSW_CPPI_RAM, CPSW_STATERAM_RX0_CP);

		/* End of Interrput vector is 1 for RX_PULSE interrupt */
		writel(CPSW_CPDMA_EOI_VECTOR_RX_PULSE, CPSW_CPDMA_EOI_VECTOR);
	}

	writel(1, CPSW_WR_C0_RX_EN);

	return 0;

}

static void cpsw_enable_rx_int(void)
{
	/* Enable rx interrupt for channel 0 */
	writel(1 << CPSW_CPDMA_RX_INTMASK_SET_RX0_PEND_MASK_BIT, CPSW_CPDMA_RX_INTMASK_SET);

	/* Enable RX_PULSE interrupt */
	writel(1, CPSW_WR_C0_RX_EN);
}

static void cpsw_register_rx_int(struct cpsw_priv *priv)
{
	printf("Enabling CPSW RX interrupt\n");
	request_irq(CPSW_3PGSWRXINT0, cpsw_irq, priv);

	cpsw_enable_rx_int();
}

int cpsw_init(void)
{
	static struct cpsw_priv priv;

	priv.rx_desc = (void *)CPSW_CPPI_RAM;
	priv.tx_desc = (void *)CPSW_CPPI_RAM + sizeof(struct desc) * 12;

	cpsw_configure_ctrl_module();

	cpsw_ss_softreset();
	cpsw_sl1_softreset();
	cpsw_cpdma_softreset();

	cpsw_mdio_init();

	cpsw_ale_init();

	/* MAC CPSW_RX_MAXLEN set to 0x5ee */
	writel(CPSW_RX_MAX_PKT_LEN, CPSW_SL1_RX_MAXLEN);

	cpsw_set_mac();

	writel((1 << CPSW_SL1_MACCONTROL_IFCTL_A_BIT) | (1 << CPSW_SL1_MACCONTROL_GMII_EN_BIT) | (1 << CPSW_SL1_MACCONTROL_FULLDUPLEX_BIT), CPSW_SL1_MACCONTROL);

	cpsw_cpdma_softreset();

	cpsw_rx_desc_init(priv.rx_desc);
	cpsw_tx_desc_init(priv.tx_desc);

	/* Write RX desc pointer */
	cpsw_write_rx_hdp(priv.rx_desc);
	/* Write TX desc pointer */
	cpsw_write_tx_hdp(priv.tx_desc);

	cpsw_register_rx_int(&priv);

	cpsw_cpdma_enable_tx();
	cpsw_cpdma_enable_rx();

	return 0;
}

#endif /* NET */
