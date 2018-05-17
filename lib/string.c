#include<lib/string.h>

int strlen(char *str)
{
    int count;

    while(*str++ && ++count);

    return count;
}

int strcmp(char *str1, char *str2)
{
    while (*str1 && *str2) {
	if (*str1 > *str2)
	    return 1;
	else if (*str1 < *str2)
	    return -1;
	str1++, str2++;
    }

    return 0;
}

void memset(char *str, char ch, int size)
{
    int one_byte_copy, i;

    one_byte_copy = size % 16;			// 16 is the block size for copy

    if (size >= 16)
        asm_memset(str, ch, size - one_byte_copy);

    str = str + size - one_byte_copy;

    for (i = 0 ; i < one_byte_copy ; i++)
	 *(str + i) = ch;

    return;
}

