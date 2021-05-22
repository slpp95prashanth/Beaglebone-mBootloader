#include <asm/types.h>
#include <stdio.h>
#include <net/lib.h>
#include <net/ip.h>

#ifdef NET

void print_ipv4_header(void *buf)
{
	struct iphdr *ip = (struct iphdr *)buf;

	printf("\n-------------- ip header ---------------------\n");
	printf("header len = 0x%02x\n", ip->ihl);
	printf("version = 0x%02x\n", ip->version);
	printf("tos = 0x%02x\n", ip->tos);
	printf("packet len including header = 0x%04x\n", ntohs(ip->tot_len));
	printf("id = 0x%04x\n", ntohs(ip->id));
	printf("frag_off = 0x%04x\n", ntohs(ip->frag_off));
	printf("ttl = 0x%02x\n", ip->ttl);
	printf("protocol = 0x%02x ", ip->protocol);

	switch (ip->protocol) {
		case 0x01:
			printf("ICMP\n");
			break;
		case 0x06:
			printf("TCP\n");
			break;
		case 0x11:
			printf("UDP\n");
			break;
		default:
			printf("Unknown type\n");
	}

	printf("checksum = 0x%04x\n", ntohs(ip->check));
	printf("src ip addr = 0x(%02x.%02x.%02x.%02x)\n", ip->saddr[0], ip->saddr[1], ip->saddr[2], ip->saddr[3]);
	printf("dest ip addr = 0x(%02x.%02x.%02x.%02x)\n", ip->daddr[0], ip->daddr[1], ip->daddr[2], ip->daddr[3]);
	printf("\n-------------- ip header ---------------------\n");
}

#endif /* NET */
