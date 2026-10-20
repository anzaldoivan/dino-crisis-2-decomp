#include "common.h"

int func_80034D60(unsigned int *bits, unsigned int n)
{
    bits = &bits[n >> 5];
    return (*bits & (1 << (n & 0x1F))) != 0;
}
