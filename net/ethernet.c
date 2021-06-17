#include <asm/types.h>
#include <stdio.h>
#include <net/net.h>
#include <net/lib.h>
#include <net/debug.h>
#include <net/ethernet.h>

#ifdef NET

void print_mac_address(unsigned char *mac)
{
	int i;

	for (i = 0; i < 6; i++) {
		printf("%02x:", mac[i]);
	}

	/* remove the last : */
	printf("\b ");
}

void print_ethhdr_header(void *buf)
{
	struct ethhdr *eth = (struct ethhdr *)buf;

	printf("\n-------------- ethernet header ---------------\n");
	printf("dest-mac = ");
	print_mac_address(eth->dest);
	printf("\nsrc-mac = ");
	print_mac_address(eth->src);

	printf("\nprotocol = 0x%04x ", ntohs(eth->proto));

	switch (ntohs(eth->proto)) {
	case 0x0800:
		printf("IPV4\n");
		break;
	case 0x0806:
		printf("ARP\n");
		break;
	default:
		printf("Unknown type\n");
	}

	printf("----------------------------------------------\n\n");

}

void ethernet_input(char *pkt, uint32_t len)
{
	print_ethhdr_header(pkt);
	
	switch (ntohs(((struct ethhdr *)pkt)->proto)) {
	case 0x0800:
		print_ipv4_header(pkt + sizeof(struct ethhdr));
		break;
	case 0x0806:
		//cpsw_send(pkt, len);
		print_arp_header(pkt + sizeof(struct ethhdr));
		break;
	default:
		printf("Unknown type\n");
	}

}

#endif /* NET */
