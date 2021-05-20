#include <stdio.h>
#include <asm/types.h>

struct ethhdr {
	unsigned char dest[6];
	unsigned char src[6];
	uint16_t proto;
}__attribute__((packed));

