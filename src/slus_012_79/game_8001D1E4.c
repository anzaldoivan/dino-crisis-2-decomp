#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x4C];
    unsigned char b4C;
    char pad4d[2];
    unsigned char b4F;
} Obj0x8001d1e4;

void func_8001D1E4(void) {
    Work *w = WORK;
    int *p = (int *)w->x5E4;
    Obj0x8001d1e4 *o;

    w->x5E8 = *p++;
    WORK->x5E4 = (int)p;
    o = (Obj0x8001d1e4 *)w->x318;
    o->b4C = 0xFF;
    o->b4F = 0;
    WORK->x3E6 = WORK->x3E7 = 0xFF;
}
