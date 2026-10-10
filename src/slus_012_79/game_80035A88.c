#include "common.h"

extern unsigned char D_800186F8[];

unsigned char func_80035A88(unsigned char a, unsigned char b)
{
    return b + D_800186F8[a];
}
