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

typedef struct Rec0x80024420 {
    char pad0[0xF];
    unsigned char xF;
    char pad10[0x28 - 0x10];
} Rec0x80024420;
typedef struct Anim0x80024420 {
    Rec0x80024420 *p;
    unsigned char x4;
    unsigned char x5;
    unsigned char x6;
    unsigned char x7;
    char pad8[4];
    int xC;
} Anim0x80024420;
typedef struct Obj0x80024420 {
    char pad0[3];
    unsigned char x3;
    char pad4[8];
    Anim0x80024420 xC;
} Obj0x80024420;
void func_800244AC(Obj0x80024420 *o);

void func_80024420(Obj0x80024420 *o)
{
    Anim0x80024420 *a;
    Rec0x80024420 *r;

    a = &o->xC;
    r = a->p;
    a->p = r + a->x4;
    if (a->xC & 1) {
        a->x6 = r->xF;
    } else {
        a->x6 = 0;
    }
    a->x7 = a->x4;
    if (a->x4 >= a->x5) {
        a->x5 = 0xFF;
    }
    o->x3++;
    func_800244AC(o);
}

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

extern void *func_80081CA4();
extern void func_80024E50();

void func_8002485C(Obj_800247D8 *p)
{
    Sub_800247D8 *s;
    int buf[2];
    int *b;

    s = &p->sub;
    if ((s->pad0[0xC] & 2) || (s->pad0[0xC] & 1)) {
        s->pad11 = s->pad11 + s->g;
        if ((unsigned char)(s->pad11 + 50) > 100) {
            s->g = s->g * -1;
        }
    }
    if (!(s->pad0[0xC] & 4)) {
        s->h -= s->pad0[7];
        if (s->h < 0) {
            s->h += 0x2580;
        }
    }
    b = buf;
    if (s->pad0[0xC] & 8) {
        s->f = (s->f + 1) & 0x3F;
    }
    func_80081CA4(b, 0, 8);
    func_80024E50(b);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80024944);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80024E50);

extern Ent0x80023ed8 *func_80047714(void);
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
extern void func_80079AA4(SVECTOR *r, MATRIX *m);
extern void func_80047584();
extern Rec0x80023ed8 D_80087628[][17];
typedef struct Ctx0x800250e0 {
    short x, y, z, pad6;
    SVECTOR rot;
    short cnt;
} Ctx0x800250e0;
typedef struct Obj0x800250e0 {
    unsigned char b0, b1, b2, b3;
    unsigned char pad4[8];
    Ctx0x800250e0 c;
} Obj0x800250e0;

void func_800250E0(Obj0x800250e0 *a)
{
    MATRIX m;
    Ctx0x800250e0 *c;
    Rec0x80023ed8 *r;
    Ent0x80023ed8 *e;
    P80x80023ed8 *w;
    SVECTOR *v;

    c = &a->c;
    r = D_80087628[a->b2];
    while (r->v8.q <= c->cnt) {
        if (r->u0.w == -1) {
            func_80047584(a);
            break;
        }
        if (c->cnt == r->v8.q) {
            e = func_80047714();
            if (e == 0)
                break;
            e->x58 = 4;
            e->x6C = r->u0.b.b1;
            e->x6E = r->u0.b.b2;
            e->x84 = r->u0.b.b3;
            e->x64 = r->x4;
            e->x66 = r->x6;
            e->x40 = r->v8;
            e->x40.x += c->x;
            e->x40.y += c->y;
            e->x40.z += c->z;
            e->x7C = r->v10;
            e->x7A = r->x1C;
            w = &e->x98;
            e->x98.a = r->x20.a;
            w->b = r->x20.b;
            v = (SVECTOR *)&e->x7C;
            if (a->b2 == 1) {
                func_80079AA4(&c->rot, &m);
                func_80079704(&m, v, v);
                *(short *)&w->b += c->rot.vy;
            }
        }
        r++;
    }
    c->cnt++;
}

typedef struct Sub0x800252d0 {
    char pad0[0x10];
    unsigned char x10;
    char pad11;
    unsigned char x12;
    char pad13[2];
    unsigned char x15;
} Sub0x800252d0;
typedef struct Obj0x800252d0 {
    char pad0[3];
    signed char x3;
    char pad4[0xC - 4];
    Sub0x800252d0 sub;
} Obj0x800252d0;
extern void func_8002628C();
extern void func_800254B8();
extern void (*D_80087CD0[])(Obj0x800252d0 *);
extern unsigned char D_80087CCC;

void func_800252D0(Obj0x800252d0 *arg0)
{
    Work *w = WORK;
    Sub0x800252d0 *s;
    unsigned char *u;
    int flag;

    u = (unsigned char *)w->units;
    s = &arg0->sub;
    if ((w->xC50 & 0xF) != 5) {
        arg0->x3 = 2;
    }
    flag = 0;
    if (s->x15 && (u[0x55] == 6 || (u[0x55] == 5 && (unsigned int)(u[0x56] - 5) < 2))) {
        flag = 1;
    }
    if (!flag) {
        s->x10 = 0;
        s->x12 = 0;
        s->x15 = 0;
        D_80087CCC = 1;
    }
    D_80087CD0[arg0->x3](arg0);
    if (flag) {
        func_8002628C(arg0);
        func_800254B8(arg0);
    }
}

typedef struct {
    void *x0;
    char *x4;
} Sub0x800253c0;
typedef struct {
    unsigned char pad0[3];
    unsigned char x3;
    void (*x4)();
    unsigned char pad8[4];
    Sub0x800253c0 sub;
} Obj0x800253c0;
typedef struct {
    short m[9];
    char pad12[0x48 - 0x12];
} Mat0x800253c0;
typedef struct {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    char pad4[0xA - 4];
    short xA;
    char padC[2];
    short xE;
} Cam0x800253c0;
extern Mat0x800253c0 D_800AF668[];
extern void func_80025B44();

void func_800253C0(Obj0x800253c0 *arg0)
{
    Sub0x800253c0 *s = &arg0->sub;
    Mat0x800253c0 *p;
    Cam0x800253c0 *c;

    s->x0 = D_80182398;
    func_80018020(D_800AF668, 1);
    s->x4 = func_800474B4(WORK->units->id, 3);
    arg0->x4 = func_80025B44;
    arg0->x3++;
    for (p = D_800AF668; p < &D_800AF668[32]; p++) {
        p->m[8] = 0x1000;
        p->m[4] = 0x1000;
        p->m[0] = 0x1000;
        p->m[7] = 0;
        p->m[6] = 0;
        p->m[5] = 0;
        p->m[3] = 0;
        p->m[2] = 0;
        p->m[1] = 0;
    }
    c = (Cam0x800253c0 *)WORK->padC;
    c->xE = 0x1000;
    c->xA = 0x7D0;
    c->x1 = 0;
    c->x2 = 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_800254B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80025780);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80025B44);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80023E5C", func_80025FC0);

typedef struct Buf0x8002628c {
    char pad0[0x40];
    SVECTOR out;
    char pad48[0x5A - 0x48];
    short x5A;
    char pad5c[0x74 - 0x5C];
    short x74;
    char pad76[2];
    short x78;
    char pad7a[0x85 - 0x7A];
    unsigned char x85;
    char pad86[0xB0 - 0x86];
} Buf0x8002628c;
typedef struct Lim0x8002628c {
    char pad0[8];
    unsigned int x8;
    short xC;
} Lim0x8002628c;
extern int func_80025FC0(Buf0x8002628c *);
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
extern void func_80079AA4(SVECTOR *r, MATRIX *m);

void func_8002628C(char *arg0)
{
    Buf0x8002628c b;
    MATRIX m;
    SVECTOR r;
    Unit *u;
    Lim0x8002628c *p;
    int v;

    u = WORK->units;
    p = (Lim0x8002628c *)(arg0 + 0xC);
    b.x74 = u->x40;
    b.x78 = u->x44;
    p->x8 = 5000;
    b.x85 = 0x18;
    r.vx = 0;
    r.vy = u->x42 - 0x47E;
    r.vz = 3000;
    b.x5A = r.vy / -1000;
    func_80079AA4(&u->x48, &m);
    func_80079704(&m, &r, &b.out);
    b.out.vx += u->x40;
    b.out.vz += u->x44;
    v = func_80025FC0(&b);
    if (v) {
        p->x8 = v;
    }
    if (p->x8 < p->xC) {
        p->xC = p->x8;
    }
}

typedef Elem0x8005e6a4 Ent0x800263a8;
extern Mat0x800253c0 D_800AF668[];
extern void func_80018020(void *, int);

Ent0x800263a8 *func_800263A8(unsigned char n)
{
    Ent0x800263a8 *p = &((Ent0x800263a8 *)WORK->x4E8)[n];

    func_80018020(p, 0xD);
    func_80018020(D_800AF668, 0x240);
    p->x0 = 1;
    p->x1 = 5;
    return p;
}

typedef struct {
    char pad0[0x34];
    int x34;
    char pad38[0x48 - 0x38];
} Mat0x8002641c;
extern Mat0x800253c0 D_800AF668[];

void func_8002641C(void)
{
    Mat0x8002641c *p;
    unsigned char *e = (unsigned char *)WORK->x4E8 + 0xC;

    for (p = (Mat0x8002641c *)D_800AF668; p < (Mat0x8002641c *)&D_800AF668[32]; p++) {
        p->x34 = 0;
    }
    e[0x15] = 0;
}

#define SHARED_D0 D_80087DE0
void func_8002645C(Obj80023E04_s80023E04 *obj)
{
#include "../shared/func_80023E04.inc.c"
}
