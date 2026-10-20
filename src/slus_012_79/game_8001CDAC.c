#include "common.h"

#include "dc2.h"
extern Obj0x8001cc74 D_800ABA08[4];

void func_8001CDAC(int i, int v)
{
    Obj0x8001cc74 *e = &D_800ABA08[i];

    if (e->f0 == 0) {
        e->f4 = v;
        e->f0 = 1;
    }
}
