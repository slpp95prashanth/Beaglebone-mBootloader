#include <asm/types.h>
#include <net/lib.h>
#include <stdio.h>

/* swap a byte in 16 bytes of data */
#define byteswap16(data)	((((data) >> 8) & 0x00ff) | (((data) << 8) & 0xff00))

uint32_t ntohl(uint32_t data)
{
	return (((byteswap16(data) << 16) & 0xffff0000) | ((byteswap16(data >> 16)) & 0x0000ffff));
}

uint16_t ntohs(uint16_t data)
{
	return byteswap16(data);
}

uint32_t htonl(uint32_t data)
{
	return (((byteswap16(data) << 16) & 0xffff0000) | ((byteswap16(data >> 16)) & 0x0000ffff));
}

