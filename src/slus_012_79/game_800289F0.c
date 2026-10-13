#include "common.h"
#include "dc2.h"

extern int D_800AF11C;
extern void func_80047B8C(void *, void *);

void func_800289F0(void *arg0, unsigned char idx, void *arg2, short x40, short vy)
{
    Unit *u;

    u = &WORK->units[idx + 1];
    u->flags = 3;
    u->x40 = x40;
    u->x48.vz = 0;
    u->x48.vx = 0;
    u->xF4 = 0;
    u->xF2 = 0;
    u->xF0 = 0;
    u->x5A = 0;
    u->x44 = 0;
    u->x42 = 0;
    u->x48.vy = vy;
    D_800AF11C++;
    func_80047B8C(u, arg2);
}
