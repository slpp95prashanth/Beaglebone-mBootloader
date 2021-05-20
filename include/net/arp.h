#include <stdio.h>
#include <asm/types.h>

struct arphdr {
	uint16_t ar_hrd;		/* format of hardware address	*/
	uint16_t ar_pro;		/* format of protocol address	*/
	unsigned char ar_hln;		/* length of hardware address	*/
	unsigned char ar_pln;		/* length of protocol address	*/
	uint16_t ar_op;		/* ARP opcode (command)		*/
	unsigned char ar_sha[6];	/* sender hardware address	*/
	unsigned char ar_sip[4];		/* sender IP address		*/
	unsigned char ar_tha[6];	/* target hardware address	*/
	unsigned char ar_tip[4];		/* target IP address		*/
} __attribute__((packed));

