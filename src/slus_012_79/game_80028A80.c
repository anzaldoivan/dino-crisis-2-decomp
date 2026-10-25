#include "common.h"
#include "dc2.h"

extern void func_80018020(void *, int);
extern void func_8001D89C();
extern void func_80079944();
int func_80079AA4(SVECTOR *r, MATRIX *m);

void func_80028A80(short *a)
{
    Work *w = WORK;
    Cam0x800266dc *c = (Cam0x800266dc *)w->x318;
    Unit *u = w->units;
    SVECTOR r;

    func_80018020(c, 0x22);
    c->x28 = a[0];
    c->x2A = a[1];
    c->x2C = a[2];
    c->x38[0] = a[3];
    c->x38[1] = a[4];
    c->x38[2] = a[5];
    func_8001D89C(&c->x28, c->x38, &c->x20, c->x30);
    r.vx = -c->x20;
    r.vy = -c->x22;
    r.vz = -c->x24;
    func_80079AA4(&r, (MATRIX *)c);
    func_80079944(0x140);
    u->x40 = a[6];
    u->x42 = a[7];
    u->x44 = a[8];
}
