#include "common.h"
#include "dc2.h"

extern int func_8004A0F8(Pos0x8001d338 *a, Pos0x8001d338 *b);
extern int func_8004A144(int v);
extern int func_8007AC34(int, int);

void func_8004A240(Pos0x8001d338 *a, Pos0x8001d338 *b, Pos0x8001d338 *out)
{
    int d = func_8004A144(func_8004A0F8(a, b));

    out->x = func_8007AC34(b->y - a->y, d);
    out->y = func_8007AC34(b->x - a->x, b->z - a->z) + 0x800;
    out->z = 0;
}
