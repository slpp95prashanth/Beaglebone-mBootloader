#include <stdio.h>

void print_packet(void *buf, uint32_t len)
{
	int data, temp, i;

	for (i = 0; i < len; i = i + 4) {
		temp = *(volatile unsigned int *)((char *)buf + i);
		data = (temp << 24) | ((temp & 0x0000ff00) << 8) | ((temp & 0x00ff0000) >> 8) | ((temp & 0xff000000) >> 24);
		printf("%08x ", data);
	}

	printf("\n");
}

