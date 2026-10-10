#include "dc2.h"
#include "../shared/func_80023E04.h"

#define SHARED_D0 D_80087600
void func_80023E5C(Obj80023E04_s80023E04 *obj)
{
#include "../shared/func_80023E04.inc.c"
}

typedef struct Sub0x80023e98 {
    char pad0[9];
    unsigned char f9;
    unsigned char fA;
    unsigned char fB;
    unsigned char fC;
} Sub0x80023e98;
typedef struct Obj0x80023e98 {
    char pad0[3];
    unsigned char f3;
    char pad4[0xC - 4];
    Sub0x80023e98 sub;
} Obj0x80023e98;
void func_80023ED8(Obj0x80023e98 *arg0);

void func_80023E98(Obj0x80023e98 *arg0)
{
    Sub0x80023e98 *s = &arg0->sub;
    s->fA = 0;
    s->fC = 0;
    s->fB = s->f9;
    arg0->f3++;
    func_80023ED8(arg0);
}

typedef struct {
    char pad0[2];
    unsigned char f2;
    unsigned char f3;
    char pad4[8];
    Sub0x80023ed8 sub;
} Obj0x80023ed8;
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
    char pad85;
    unsigned char x86;
    unsigned char x87;
    char pad88[0x8C - 0x88];
    Obj0x80023ed8 *x8C;
    char pad90[0x98 - 0x90];
    P80x80023ed8 x98;
    char pada0[0xAC - 0xA0];
} Ent0x80023ed8;
struct Obj0x80023e98;
extern unsigned int D_800AF590;
extern void func_80018020(void *, int);
extern char *func_800474B4(int a, int b);
extern Ent0x80023ed8 *func_80047714(void);
extern void func_80047814(Ent0x80023ed8 *);

void func_80023ED8(struct Obj0x80023e98 *arg0)
{
    Obj0x80023ed8 *o = (Obj0x80023ed8 *)arg0;
    Sub0x80023ed8 *s = &o->sub;
    Rec0x80023ed8 *r = s->rec;
    int base = o->sub.x0;
    Ent0x80023ed8 *e;
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
                e = &((Ent0x80023ed8 *)WORK->x4E4)[s->xB];
                if (e->x50 & 1) {
                    goto next;
                }
                func_80018020(e, 0x2B);
                e->x50 = 3;
                e->x67 = 0x2C;
                e->x8C = o;
                e->x86 = e->x87 = s->xB;
                mode = (int)&e->x98;
                s->xC++;
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
                e->x8C = o;
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
    s->xC--;
    o->f3++;
    return;
bump:
    o->f3++;
}

typedef struct {
    unsigned char pad0[3];
    unsigned char b3;
    unsigned char pad4[0x18 - 4];
    unsigned char b18;
} Obj0x8002438c;

void func_8002438C(Obj0x8002438c *p)
{
    if (p->b18 == 0) {
        p->b3++;
    }
}

typedef struct {
    unsigned char pad0[3];
    signed char state;
    unsigned char pad4[8];
    int *x0C;
} Obj0x800243b4;
extern void (*D_80087610[])(Obj0x800243b4 *);

void func_800243B4(Obj0x800243b4 *p)
{
    D_80087610[p->state](p);
    if (*p->x0C == -1) {
        p->state++;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80024420);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800244AC);

#define SHARED_D0 D_8008761C
void func_8002479C(Obj80023E04_s80023E04 *obj)
{
#include "../shared/func_80023E04.inc.c"
}

typedef struct {
    unsigned char pad0[0xE];
    unsigned char e;
    unsigned char f;
    signed char g;
    unsigned char pad11;
    short h;
} Sub_800247D8;
typedef struct {
    unsigned char pad0[3];
    unsigned char cnt;
    void (*fn)();
    unsigned char pad8[4];
    Sub_800247D8 sub;
} Obj_800247D8;
extern void func_80024944();
extern void func_8002485C(Obj_800247D8 *);
extern void func_80034C48(int, int, int);

void func_800247D8(Obj_800247D8 *p)
{
    Sub_800247D8 *s;

    p->cnt++;
    s = &p->sub;
    s->h = 0;
    if (s->g == 0) {
        s->g = 5;
    }
    if (s->e == 0) {
        s->e = 2;
    }
    s->f = 0;
    p->fn = func_80024944;
    func_80034C48(3, 8, 1);
    func_8002485C(p);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_8002485C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80024944);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80024E50);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800250E0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800252D0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800253C0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800254B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80025780);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80025B44);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80025FC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_8002628C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800263A8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_8002641C);

#define SHARED_D0 D_80087DE0
void func_8002645C(Obj80023E04_s80023E04 *obj)
{
#include "../shared/func_80023E04.inc.c"
}
