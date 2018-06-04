#ifndef _STDIO_H
#define _STDIO_H

#include <asm/types.h>

typedef uint32_t size_t;

#define NULL (void *)0

#ifdef DEBUG_PRINTF
int tfp_printf(char *, ...);
#endif

#endif /* _STDIO_H */ 
