#include "common.h"
#include "dc2.h"

int func_800786B4(u8 *p)
{
    return ((((p[0] >> 4) * 10 + (p[0] & 0xF)) * 60
             + ((p[1] >> 4) * 10 + (p[1] & 0xF))) * 75
            + ((p[2] >> 4) * 10 + (p[2] & 0xF))) - 150;
}
