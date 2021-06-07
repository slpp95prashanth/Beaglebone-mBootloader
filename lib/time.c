#include <asm/types.h>
#include <stdio.h>

#include <timer/timer.h>

void system_uptime(void)
{
	struct timeval time;

	uint32_t hrs = 0, min = 0, sec = 0;

	gettimeofday(&time);

	while (time.tv_sec - (hrs * 3600) >= 3600) {
		hrs++;
	}

	while ((time.tv_sec - hrs * 3600) - min * 60 >= 60) {
		min++;
	}

	sec = time.tv_sec - hrs * 3600 - min * 60;

	printf("%02x:%02x:%02x\n", hrs, min, sec);
}
