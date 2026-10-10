#include "common.h"

extern unsigned char D_800AF5C0[];

void func_8001F990(unsigned char arg0)
{
    unsigned char *p = D_800AF5C0;

    p[0x6D] = 0;
    if (arg0 == 2) {
        p[0x6C]++;
    }
}
