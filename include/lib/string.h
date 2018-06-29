#ifndef _LIB_STRING_H
#define _LIB_STRING_H

#include <stdio.h>
#include <asm/types.h>

int strncmp(const char *, const char *, int);
int strlen(const char *);
char *strncpy(char *, const char *, int);
int inttostr(int, char *, int);
size_t strspn(const char *, const char *);
char *strtok(char *str, const char *delim);
char *strpbrk(const char *s, const char *accept);
char *strchr(const char *s, int c);
unsigned long int simple_strtoul(const char *nptr, char **endptr, int base);

void asm_memset(void *, char, int);
void asm_memcpy(void *, const void *, int);
void memset(char *, char, int);
void memcpy(char *, char *, int);

#endif /* _LIB_STRING_H */
