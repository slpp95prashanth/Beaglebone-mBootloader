#include<lib/string.h>

int strlen(const char *str)
{
    int count;

    count = 0;

    while(*str++ && ++count);

    return count;
}

char *strncpy(char *dest, const char *src, int count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != '\0') {
			src++;
			tmp++;
			count--;
		}
	}

    return dest;
}

int strncmp(const char *str1, const char *str2, int count)
{
    while (count && *str1 && *str2) {
	if (*str1 > *str2)
	    return 1;
	else if (*str1 < *str2)
	    return -1;

	str1++, str2++;

	count--;
    }

    return 0;
}

int inttostr(int num, char *str, int type)
{
    int rem, i;
    int count;

    char str1[12];

    i = 11;
    count = 0;

    str1[i--] = '\0';
    
    if (type == 16) {
        while (num) {
            rem = num % 16;
            num = num / 16;
 
            if (rem >= 0 && rem <= 9) {
                str1[i--] = '0' + rem;
            } else if (rem >= 10 && rem <= 15) {
                str1[i--] = 'a' + rem - 10;
            } else {
                return -1;
            }

	count++;

        }
    }

    strncpy(str, str1 + i + 1, count);
    return count;
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

void memcpy(char *dst, char *src, int size)
{
    int one_byte_copy, i;

    one_byte_copy = size % 16;			// 16 is the block size for copy

    if (size >= 16)
        asm_memcpy(dst, src, size - one_byte_copy);

    dst = (char *)dst + size - one_byte_copy;
    src = (char *)src + size - one_byte_copy;

    for (i = 0 ; i < one_byte_copy ; i++) {
	*((char *)dst + (i)) = *((char *)src + (i));
    }

    return;
}

