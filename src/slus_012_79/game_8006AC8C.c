#include "common.h"
#include "dc2.h"

/* func_8006AC8C: VECTOR, Unit, Work, WORK, Elem0x8006ac8c, Obj0x8006ac8c in dc2.h */

typedef struct {
    char pad0[0x1C];
    Unit *p1C;
    char pad20[0x34 - 0x20];
    unsigned char b34[1];
} S0x8006ac8c;
extern int D_800AF11C;
extern S0x8006ac8c D_800B53C8;
extern int func_80078844(int);
extern int func_80078914(int);
extern void func_8007AFA4(VECTOR *a, VECTOR *b, VECTOR *c);

void func_8006AC8C(void)
{
    Obj0x8006ac8c *p;
    Elem0x8006ac8c *e;
    VECTOR a, b, c;
    short x, z, dx, dz;
    short i;
    int s;
    Unit *u;

    p = (Obj0x8006ac8c *)WORK->x318;
    e = &p->x40[p->b4C];
    x = (e->h6 * func_80078844(p->h22)) / 4096;
    z = (e->h6 * func_80078914(p->h22)) / 4096;
    x += func_80078914(p->h22) * 120 / 4096;
    s = func_80078844(p->h22);
    s = (s - s * 16) * 8;
    z += s / 4096;
    dx = func_80078844(p->h22) * 2000 / 4096;
    dz = func_80078914(p->h22) * 2000 / 4096;
    a.vz = z;
    a.vx = x;
    a.vy = 0;
    D_800B53C8.p1C = 0;
    for (i = 1; (unsigned)i < (unsigned)D_800AF11C; i++) {
        if ((WORK->units[i].flags & 1) && (WORK->units[i].x184 & 1)) {
            b.vx = (WORK->units[i].x40 - WORK->units[0].x40 + dx) / 8;
            b.vz = (WORK->units[i].x44 - WORK->units[0].x44 + dz) / 8;
            b.vy = 0;
            func_8007AFA4(&a, &b, &c);
            if (c.vy > 0) {
                D_800B53C8.b34[i] |= 2;
                u = D_800B53C8.p1C;
                if (u == 0) {
                    D_800B53C8.p1C = &WORK->units[i];
                } else if (u->x1C8 > WORK->units[i].x1C8) {
                    D_800B53C8.p1C = &WORK->units[i];
                }
            }
        }
    }
}
