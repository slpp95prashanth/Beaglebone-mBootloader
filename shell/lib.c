#include <lib/string.h>

int ishex(char ch)
{
	if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')) {
		return 0;
	}

	return -1;
}

int check_address_validity(char *addr)
{
	int len, i = 0;

	len = strlen(addr);

	if (len != 8) {
		if (addr[0] != '0' && addr[1] != 'x') {
			return -1;
		} else {
			i = 2;
		}
	}

	for (;i <= 8; i++) {
		if (ishex(addr[i]) != 0)
			return -1;
	}

	return 0;
}

