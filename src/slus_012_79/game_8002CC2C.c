#include "dc2.h"

typedef struct {
    char pad0[0x54];
    unsigned char f3;
    char pad55[0x7A - 0x55];
    unsigned char f2;
    char pad7B[0x98 - 0x7B];
    Sub0x80023ed8 sub;
} Obj0x8002cc2c;
typedef struct {
    char pad0[0x40];
    V80x80023ed8 x40;
    char pad48[0x50 - 0x48];
    int x50;
    char pad54[0x58 - 0x54];
    unsigned char x58;
    char pad59[0x60 - 0x59];
    void *x60;
    unsigned short x64;
    unsigned char x66;
    unsigned char x67;
    int x68;
    unsigned short x6C;
    unsigned short x6E;
    int x70;
    char pad74[0x7A - 0x74];
    unsigned short x7A;
    W80x80023ed8 x7C;
    unsigned char x84;
    char pad85[0x98 - 0x85];
    P80x80023ed8 x98;
    char pada0[0xAC - 0xA0];
} Ent0x8002cc2c;
extern unsigned int D_800AF590;
extern void func_80018020(void *, int);
extern char *func_800474B4(int a, int b);
extern Ent0x8002cc2c *func_80047714(void);
extern void func_80047814(Ent0x8002cc2c *);

void func_8002CC2C(Obj0x8002cc2c *o)
{
    Sub0x80023ed8 *s = &o->sub;
    Rec0x80023ed8 *r = s->rec;
    int base = o->sub.x0;
    Ent0x8002cc2c *e;
    int mode;

    if (s->xA < r->v8.q) {
        goto after;
    }
    do {
        if (r->u0.w == -1) {
            if (o->f2 & 8) {
                goto wrap;
            }
            o->f3++;
            return;
        }
        if (s->xA == r->v8.q) {
            if (!(o->f2 & 1)) {
                e = &((Ent0x8002cc2c *)WORK->x4E4)[s->xB];
                if (e->x50 & 1) {
                    goto next;
                }
                func_80018020(e, 0x2B);
                e->x50 = 3;
                e->x67 = 0x2C;
                s->xC++;
                mode = (int)&e->x98;
                if (++s->xB >= D_800AF590) {
                    o->f3++;
                    return;
                }
            } else {
                e = func_80047714();
                if (e == 0) {
                    goto after;
                }
                mode = (int)&e->x98;
                s->xC++;
                s->xB++;
            }
            e->x58 = r->u0.b.b0;
            e->x6C = r->u0.b.b1;
            e->x6E = r->u0.b.b2;
            e->x64 = r->x4;
            e->x66 = r->x6;
            e->x40 = r->v8;
            e->x7C = r->v10;
            e->x7A = r->x1C;
            e->x84 = r->u0.b.b3;
            ((P80x80023ed8 *)mode)->a = r->x20.a;
            ((P80x80023ed8 *)mode)->b = r->x20.b;
            if (e->x84 & 0x40) {
                e->x67 |= 2;
            }
            mode = r->x1E >> 4;
            if (o->f2 & 2) {
                e->x40.x += s->xE;
                e->x40.y += s->x10.h.a;
                e->x40.z += s->x10.h.b;
                e->x68 = r->x18;
                e->x70 = base + r->v10.d;
                mode = 1;
            } else if (o->f2 & 4) {
                e->x64 = s->x10.h.a;
                e->x66 = s->x10.h.b;
                e->x68 = r->x18;
                e->x70 = base + r->v10.d;
            } else if (s->x10.h.a != 0) {
                e->x68 = s->x10.w;
                e->x70 = base + r->v10.d;
            } else {
                e->x68 = r->x18;
                e->x70 = base + r->v10.d;
            }
            if (mode & 4) {
                if (!(((Shot0x80023ed8 *)WORK->x4DC)[s->x8 + 12].x50 & 1)) {
                    goto kill;
                }
                e->x60 = &((Shot0x80023ed8 *)WORK->x4DC)[s->x8 + 12];
            } else if (mode == 0) {
                if (!(WORK->units[s->x8].flags & 1)) {
                    goto kill;
                }
                e->x60 = func_800474B4(WORK->units[s->x8].id, r->x1F);
            }
        }
    next:
        s->rec++;
        r++;
    } while (r->v8.q <= s->xA);
after:
    s->xA++;
    if (o->sub.rec->u0.w != -1) {
        return;
    }
    if (!(o->f2 & 8)) {
        goto bump;
    }
    s->xA = -1;
    s->rec -= s->xB - s->x9;
    s->xB = s->x9;
    return;
wrap:
    s->xA = 0;
    s->rec -= s->xB - s->x9;
    s->xB = s->x9;
    return;
kill:
    func_80047814(e);
bump:
    o->f3++;
}
