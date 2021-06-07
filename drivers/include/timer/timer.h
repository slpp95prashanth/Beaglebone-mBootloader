#ifndef _TIMER_
#define _TIMER_

#include <asm/types.h>

void timer_irq_init(void);
void timer_init(int, void *);

struct timer {
	uint32_t usecs;
	int (*func)(void *);
};

#define MAX_TIMER_IN_USE	(5)

#define TIMER_STOP	(0)

#define DEFAULT_TIMER_USECS	(10000)

extern uint32_t time_of_day(void *);

struct timeval {
	uint64_t tv_sec;
	uint64_t tv_usec;
};

#endif /* _TIMER_ */
