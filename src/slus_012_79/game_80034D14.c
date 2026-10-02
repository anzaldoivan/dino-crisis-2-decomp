#include "common.h"

void func_80034D14(unsigned int *bits, unsigned int n, int val);

void func_80034D14(unsigned int *bits, unsigned int n, int val)
{
    unsigned int *p = &bits[n >> 5];

    if (val == 0) {
        *p &= ~(1 << (n & 0x1F));
    } else {
        *p |= 1 << (n & 0x1F);
    }
}
