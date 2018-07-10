#include <stdio.h>

int imod_less_than_32(int num, int den)
{
    while (num > den) {
	num -= den;
    }

    return num;
}

int idiv_by_pow16(unsigned  num, unsigned den)
{
    int count = 0;

    while (den >> count != 1) {
        count++;
    }

    return num >> count;
}

int imod_by_pow16(unsigned num, unsigned den)
{
    return num & (den - 1);
}

