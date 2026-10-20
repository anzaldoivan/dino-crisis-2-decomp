#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x94];
    int x94;
    char pad98[0xC0 - 0x98];
} Ent0x8001cc18;
extern Obj0x8001cc74 D_800ABA08[4];
extern char D_800A89F4[4][0x1000];

void func_8001CC18(void)
{
    Obj0x8001cc74 *p;
    Ent0x8001cc18 *e;
    unsigned int i;
    Ent0x8001cc18 **b;

    p = D_800ABA08;
    b = D_00000110;
    for (i = 0; i < 4; i++) {
        p->f0 = 0;
        e = &(*b)[i] + 1;
        p->fC = (int)D_800A89F4[i];
        p->f14 = (int)e;
        e->x94 = 0x40000404;
        p++;
    }
}
