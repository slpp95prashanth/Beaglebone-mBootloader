#include <stdio.h>
#include <asm/types.h>

struct iphdr {
	char	ihl:4,
		version:4;
	char tos;
	uint16_t tot_len;
	uint16_t id;
	uint16_t frag_off;
	char ttl;
	char protocol;
	uint16_t check;
	char saddr[4];
	char daddr[4];
	/*The options start here. */
} __attribute__((__packed__));

