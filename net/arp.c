#include <stdio.h>
#include <asm/types.h>
#include <net/lib.h>
#include <net/arp.h>
#include <net/debug.h>

void print_arp_header(void *buf)
{
	struct arphdr *arp = buf;

	printf("\n-------------- arp header --------------------\n");
	printf("hardware type = 0x%04x ", ntohs(arp->ar_hrd));

	switch (ntohs(arp->ar_hrd)) {
		case 0x0001:
			printf("Ethernet\n");
			break;
		default:
			printf("Unknown type\n");
	}

	printf("protocol type = 0x%04x ", ntohs(arp->ar_pro));

	switch (ntohs(arp->ar_pro)) {
		case 0x0800:
			printf("IPv4\n");
			break;
		default:
			printf("Unknown type\n");
	}

	printf("hardware address len = 0x%02x\n", arp->ar_hln);
	printf("protocol address len = 0x%02x\n", arp->ar_pln);

	printf("sender mac address = ");
	print_mac_address(arp->ar_sha);
	printf("sender ip addr = 0x(%02x.%02x.%02x.%02x)\n", arp->ar_sip[0], arp->ar_sip[1], arp->ar_sip[2], arp->ar_sip[3]);
	printf("target mac address = ");
	print_mac_address(arp->ar_tha);
	printf("sender ip addr = 0x(%02x.%02x.%02x.%02x)\n", arp->ar_tip[0], arp->ar_tip[1], arp->ar_tip[2], arp->ar_tip[3]);
	
	printf("\n----------------------------------------------\n");
}
