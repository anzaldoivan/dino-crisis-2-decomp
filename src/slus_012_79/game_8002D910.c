#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x40];
    SVECTOR pos;
    char pad48[0x54 - 0x48];
    unsigned char x54;
    char pad55[0x5A - 0x55];
    short x5A;
    char pad5c[0x74 - 0x5C];
    int old;
    short oldz;
    unsigned char x7A;
    char pad7b;
    short velx, vely, velz;
    char pad82[0x84 - 0x82];
    unsigned char x84;
} Obj0x8002d910;
extern char D_80088F70[];
extern int func_8001C5AC();
extern int func_8003245C();
extern unsigned int func_8003252C(void *);
extern int func_80033D4C(void *, void *, SVECTOR *, int);
extern void func_8003D22C();
extern int func_80048048(void *, void *, int, int, int, int);
extern int func_80048274();
extern int func_8004A2C4(void *p, int arg);

void func_8002D910(Obj0x8002d910 *o)
{
    SVECTOR b;
    SVECTOR a;
    Unit buf;
    Unit *p;
    Unit *q;

    o->old = *(int *)&o->pos;
    o->oldz = o->pos.vz;
    o->pos.vx += o->velx;
    o->pos.vy += o->vely;
    o->pos.vz += o->velz;
    o->x5A = o->pos.vy / -1000;
    if (func_80048274(o) && !(o->x84 & 8)) {
        o->x54++;
    }
    if (o->x7A == 0) {
        return;
    }
    if (func_8003252C(o) || !func_80033D4C(&o->pos, &o->old, &o->pos, 16)) {
        func_80048048(&o->pos, D_80088F70, 0, 0, 32, 0);
        o->x54++;
        return;
    }
    a.vx = o->pos.vx;
    a.vy = o->pos.vy;
    a.vz = o->pos.vz;
    a.pad = 0xFA;
    p = WORK->units;
    b.vx = p->x40;
    b.vy = p->x42 - 0x348;
    b.vz = p->x44;
    b.pad = 0x17C;
    if (!(p->x174 & 1) && func_8003245C(&a, &b, 0x348, 0x190)) {
        func_8001C5AC(p, 7);
        p->x54 = 0x102;
    } else {
        q = WORK->x6F0;
        if (q == 0) {
            return;
        }
        p = q;
        if (p->x174 & 1) {
            return;
        }
        b.vx = p->x40;
        b.vy = p->x42 - 0x348;
        b.vz = p->x44;
        b.pad = 0x17C;
        if (!func_8003245C(&a, &b, 0x348, 0x190)) {
            return;
        }
        p->x54 = 2;
    }
    buf.x48.vy = func_8004A2C4(&o->pos, (int)&p->x40);
    func_8003D22C(&buf);
    p->x174 = 1;
    p->x176 = 0xA0;
    p->x170 = 0xFFFF;
    func_80048048(&o->pos, D_80088F70, 0, 0, 32, 0);
    o->x54++;
}
