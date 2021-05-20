#include <asm/types.h>

struct ethhdr {
	unsigned char dest[6];
	unsigned char src[6];
	uint16_t proto;
};

void ethernet_input(char *, uint32_t);
