#include "common.h"
#include "dc2.h"

extern int func_80078E14(int);

int func_800323EC(Rect0x8001b56c *a, Rect0x8001b56c *b)
{
    int dx = a->x0 - b->x0;
    int dz = a->x4 - b->x4;
    int r = a->x6 + b->x6;

    return r - func_80078E14(dx * dx + dz * dz) >= 0;
}
