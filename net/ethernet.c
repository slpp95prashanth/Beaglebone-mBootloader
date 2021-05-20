#include <asm/types.h>
#include <stdio.h>

struct ethhdr {
	unsigned char dest[6];
	unsigned char src[6];
	uint16_t proto;
};

void print_mac_address(char *mac)
{
	int i;

	for (i = 0; i < 6; i++) {
		printf("%02x:", mac[i]);
	}
}

void print_ethhdr_header(char *buf)
{
	struct ethhdr *eth = buf;

	printf("dest-mac = ");
	print_mac_address(eth->dest);
	printf("\nsrc-mac = ");
	print_mac_address(eth->src);

	printf("\nprotocol = 0x%02x\n", eth->proto);
}

void ethernet_input(char *pkt, size_t len)
{
	print_ethhdr_header(pkt);
}
