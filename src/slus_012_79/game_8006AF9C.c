#include "common.h"
#include "dc2.h"
#include "../shared/func_80023E04.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;







extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


#define SHARED_F0 func_8004A2C4

extern void func_80034C48(int, int, int);
extern void (*D_8008F7F0[])(Obj80066C20 *);


/* func_8006AF9C (T8 w08, recovered T8.c198): VECTOR, Unit, Work, WORK, Elem0x8006ac8c, Obj0x8006ac8c in dc2.h */
typedef struct {
    char pad0[0x20];
    Unit *p20;
    char pad24[0x34 - 0x24];
    unsigned char b34[1];
} S0x8006af9c;
extern S0x8006af9c D_800B53C8;
extern int func_80078844(int);
extern int func_80078914(int);
extern void func_8007AFA4(VECTOR *a, VECTOR *b, VECTOR *c);

void func_8006AF9C(void)
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
    x += -(func_80078914(p->h22) * 120) / 4096;
    s = func_80078844(p->h22);
    s = (s - s * 16) * 8;
    z += -s / 4096;
    dx = func_80078844(p->h22) * 2000 / 4096;
    dz = func_80078914(p->h22) * 2000 / 4096;
    a.vz = z;
    a.vx = x;
    a.vy = 0;
    D_800B53C8.p20 = 0;
    for (i = 1; (unsigned)i < (unsigned)D_800AF11C; i++) {
        if ((WORK->units[i].flags & 1) && (WORK->units[i].x184 & 1)) {
            b.vx = (WORK->units[i].x40 - WORK->units[0].x40 + dx) / 8;
            b.vz = (WORK->units[i].x44 - WORK->units[0].x44 + dz) / 8;
            b.vy = 0;
            func_8007AFA4(&a, &b, &c);
            if (c.vy < 0) {
                D_800B53C8.b34[i] |= 1;
                u = D_800B53C8.p20;
                if (u == 0) {
                    D_800B53C8.p20 = &WORK->units[i];
                } else if (u->x1C8 > WORK->units[i].x1C8) {
                    D_800B53C8.p20 = &WORK->units[i];
                }
            }
        }
    }
}

extern void func_8006AC8C(void *);

void func_8006B2B8(void *obj)
{
    S0x8006af9c *s;
    unsigned char *p;
    int i;

    i = 15;
    s = &D_800B53C8;
    p = &s->b34[15];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    ((void (*)(void *))func_8006AF9C)(obj);
    func_8006AC8C(obj);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006B308);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006B674);

typedef struct Poly0x8006bba8 {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} Poly0x8006bba8;

typedef struct Hud0x8006b938 {
    char pad0[0x10];
    int x10;
    short h14;
} Hud0x8006b938;
extern S0x8006af9c D_800B53C8;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
void func_8006BAC8(unsigned int n, short x, short y);

void func_8006B938(void)
{
    Poly0x8006bba8 p;
    unsigned int *pk;
    unsigned int *ot;
    short n;

    n = ((Hud0x8006b938 *)&D_800B53C8)->h14;
    if (n == 0) {
        return;
    }
    if (n < 10) {
        func_8006BAC8(n, 0x48, 0xB6);
    } else {
        func_8006BAC8(n, 0x4C, 0xB6);
    }
    func_8006BAC8(((Hud0x8006b938 *)&D_800B53C8)->x10, 0x4C, 0xC8);
    p.b0 = 0x80;
    p.code = 0x2E;
    p.g0 = 0x80;
    p.r0 = 0x80;
    p.u3 = 0x80;
    p.u1 = 0x80;
    p.v1 = 0x80;
    p.v0 = 0x80;
    p.u2 = 0x20;
    p.v3 = 0xA8;
    p.u0 = 0x20;
    p.v2 = 0xA8;
    p.tpage = 0x18;
    p.clut = 0x7F40;
    p.x2 = 0x14;
    p.x0 = 0x14;
    p.x3 = 0x74;
    p.x1 = 0x74;
    p.y1 = 0xB4;
    p.y0 = 0xB4;
    p.y3 = 0xDC;
    p.y2 = 0xDC;
    *(Poly0x8006bba8 *)PKT = p;
    ot = &D_800AC070[D_800ABA96][1];
    pk = PKT;
    *pk = *ot | 0x09000000;
    *ot = (unsigned int)pk & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x28);
}

void func_8006BBA8(unsigned char n, short x, short y);

void func_8006BAC8(unsigned int n, short x, short y)
{
    unsigned int d;
    int px;

    d = n % 10;
    n -= d;
    func_8006BBA8(d, x, y);
    if (n != 0) {
        px = x << 16;
        do {
            n /= 10;
            px -= 8 << 16;
            d = n % 10;
            n -= d;
            func_8006BBA8(d, px >> 16, y);
        } while (n != 0);
    }
}

extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];

void func_8006BBA8(unsigned char n, short x, short y)
{
    Poly0x8006bba8 p;
    unsigned int *ot;
    unsigned int *p2;

    p.r0 = p.g0 = p.b0 = 0x80;
    p.code = 0x2C;
    p.u0 = p.u2 = n * 8 + 0x28;
    p.v0 = p.v1 = 0x68;
    p.u1 = p.u3 = n * 8 + 0x30;
    p.v2 = p.v3 = 0x78;
    p.tpage = 0x18;
    p.clut = 0x7F40;
    p.x0 = x;
    p.y1 = p.y0 = y;
    p.x1 = x + 8;
    p.x2 = x;
    p.y2 = y + 16;
    p.x3 = x + 8;
    p.y3 = y + 16;
    *(Poly0x8006bba8 *)PKT = p;
    ot = &D_800AC070[D_800ABA96][1];
    p2 = PKT;
    *p2 = *ot | 0x09000000;
    *ot = (unsigned int)p2 & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x28);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006BCEC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006BD80);

extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
void func_8006C454(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe);

void func_8006C2D8(Spr0x800317fc *p, int x, int y, int tpage, int clut, unsigned int color)
{
    int n;
    unsigned int *ot;
    unsigned int *p2;

    n = p->n;
    while (n-- != 0) {
        Rect0x8006c2d8 r2 = { p->x + x, p->y + y, p->w, p->h };
        ((void (*)(Rect0x8006c2d8 *, unsigned char, unsigned char, unsigned short, unsigned int, unsigned int))func_8006C454)(&r2, p->u, p->v, clut, color & 0xFFFFFF, color >> 31);
        p++;
    }
    PKT[1] = tpage | 0xE1000400;
    PKT[2] = 0;
    ot = &D_800AC070[D_800ABA96][1];
    p2 = PKT;
    *p2 = *ot | 0x02000000;
    *ot = (unsigned int)p2 & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0xC);
}

extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];

void func_8006C454(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe)
{
    unsigned int *p;
    unsigned int *ot;

    ((Sprt0x8006c454 *)PKT)->x = rect[0];
    ((Sprt0x8006c454 *)PKT)->y = rect[1];
    ((Sprt0x8006c454 *)PKT)->w = rect[2];
    ((Sprt0x8006c454 *)PKT)->h = rect[3];
    ((Sprt0x8006c454 *)PKT)->u = u;
    ((Sprt0x8006c454 *)PKT)->v = v;
    ((Sprt0x8006c454 *)PKT)->clut = clut;
    if (abe) {
        ((Sprt0x8006c454 *)PKT)->code = color | 0x66000000;
    } else {
        ((Sprt0x8006c454 *)PKT)->code = color | 0x64000000;
    }
    ot = &D_800AC070[D_800ABA96][1];
    p = PKT;
    *p = *ot | 0x04000000;
    *ot = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x14);
}

extern void func_80034C48(int, int, int);
extern void (*D_8008F894[])(Obj0x8006c538 *);

void func_8006C538(Obj0x8006c538 *p)
{
    func_80034C48(3, 0, 0);
    func_80034C48(1, 3, 1);
    func_80034C48(1, 0, 0);
    D_8008F894[p->mode](p);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006C5AC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006C78C);

/* func_8006CB1C (T8 w22, hand splice T8.c345): PKT, Rect0x8006c2d8 in dc2.h; D_800ABA96, D_800AC070 above */
extern Rect0x8006c2d8 D_8001A758, D_8001A760, D_8001A768, D_8001A770; /* rodata seg slus_012_79_2f58: carve cannot move it into the unit */
extern void func_8007E074();
extern void func_8007E134();

void func_8006CB1C(int row, int depth)
{
    Rect0x8006c2d8 r0 = D_8001A758;
    Rect0x8006c2d8 r1 = D_8001A760;
    Rect0x8006c2d8 r2 = D_8001A768;
    Rect0x8006c2d8 r3 = D_8001A770;

    row *= 8;
    r0.y += row;
    r1.y += row;
    r2.y += row;
    r3.y += row;

    func_8007E134(PKT, &r0, 0, 0x1F9);
    func_8007E074(&D_800AC070[D_800ABA96][depth], PKT);
    PKT += 6;
    func_8007E134(PKT, &r1, 0x80, 0x1F9);
    func_8007E074(&D_800AC070[D_800ABA96][depth], PKT);
    PKT += 6;
    func_8007E134(PKT, &r2, 0, 0x1F7);
    func_8007E074(&D_800AC070[D_800ABA96][depth], PKT);
    PKT += 6;
    func_8007E134(PKT, &r3, 0x80, 0x1F7);
    func_8007E074(&D_800AC070[D_800ABA96][depth], PKT);
    PKT += 6;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006CD30);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006D7F4);

typedef struct Vec0x8006dabc {
    short x, y, z, w;
} Vec0x8006dabc;
typedef struct Ent0x8006dabc {
    char pad0[0x40];
    Vec0x8006dabc x40;
    char pad48[0x58 - 0x48];
    unsigned char x58;
    char pad59[0x6C - 0x59];
    short x6C;
    short x6E;
    char pad70[0x7A - 0x70];
    unsigned char x7A;
    char pad7b[0x7C - 0x7B];
    short x7C;
    short x7E;
    short x80;
    char pad82[0x9E - 0x82];
    short x9E;
    short xA0;
} Ent0x8006dabc;
extern void func_80044834(void *, int);
extern Ent0x8006dabc *func_80047714(void);
extern int func_8007A254();
extern int func_8007A3F4();
extern int (**D_800AF124)();
extern short D_800B5D78;

void func_8006DABC(char *arg0)
{
    char *s2;
    char *p;
    Ent0x8006dabc *e;
    MATRIX m;
    unsigned char *c;

    s2 = WORK->x318;
    p = arg0 + 0xC;
    e = func_80047714();
    if (e != 0) {
        func_80044834(WORK->units, 0x61);
        m.m[2][1] = 0;
        m.m[2][0] = 0;
        m.m[1][2] = 0;
        m.m[1][0] = 0;
        m.m[0][2] = 0;
        m.m[0][1] = 0;
        m.m[2][2] = 0xE8C;
        m.m[1][1] = 0xE8C;
        m.m[0][0] = 0xE8C;
        func_8007A3F4(-*(short *)(p + 0x10), &m);
        func_8007A254(-*(short *)(p + 0xE), &m);
        e->x7A = 3;
        e->x58 = 0x14;
        e->x40 = *(Vec0x8006dabc *)((char *)WORK->units + 0x40);
        e->x40.x = *(short *)(s2 + 0x38);
        e->x40.y = -2000;
        e->x40.z = *(short *)(s2 + 0x3C);
        e->x7C = m.m[2][0] / 2;
        e->x7E = m.m[2][1] / 2;
        e->x80 = m.m[2][2] / 2;
        e->x6E = 0x10;
        e->x6C = 0x10;
        e->x9E = 0x1C;
        c = (unsigned char *)&D_800B5D78;
        e->xA0 = c[0x1A] + 12;
        c[0x1A]++;
        if (c[0x1A] >= 3) {
            c[0x1A] = 0;
        }
        WORK->x4DC[e->xA0].x4A = *(short *)(p + 0x10);
        D_800AF124[e->x58](e);
        D_800AF124[e->x58](e);
    }
}

extern short D_800B5D78;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
extern void func_8007E194();

void func_8006DCE0(void)
{
    PolyF4_0x800689f4 p;
    unsigned int *ot;
    unsigned int *prim;

    if (D_800B5D78 == 0) {
        return;
    }
    p.code = 0x2A;
    p.r0 = 0;
    p.x0 = 0;
    p.y0 = 0;
    p.x1 = 0xFF;
    p.y1 = 0;
    p.x2 = 0;
    p.y2 = 0xF0;
    p.x3 = 0xFF;
    p.y3 = 0xF0;
    p.g0 = p.b0 = D_800B5D78;
    *(PolyF4_0x800689f4 *)PKT = p;
    ot = &D_800AC070[D_800ABA96][1];
    prim = PKT;
    *prim = *ot | 0x05000000;
    *ot = (unsigned int)prim & 0xFFFFFF;
    PKT += 6;
    p.x0 = 0xFF;
    p.y0 = 0;
    p.x1 = 0x140;
    p.y1 = 0;
    p.x2 = 0xFF;
    p.y2 = 0xF0;
    p.x3 = 0x140;
    p.y3 = 0xF0;
    *(PolyF4_0x800689f4 *)PKT = p;
    ot = &D_800AC070[D_800ABA96][1];
    prim = PKT;
    *prim = *ot | 0x05000000;
    *ot = (unsigned int)prim & 0xFFFFFF;
    PKT += 6;
    func_8007E194(PKT, 1, 0, 0x40, 0);
    *PKT = (*PKT & 0xFF000000) | (D_800AC070[D_800ABA96][1] & 0xFFFFFF);
    D_800AC070[D_800ABA96][1] = (D_800AC070[D_800ABA96][1] & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT += 3;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006AF9C", func_8006DF00);

void func_8006E3AC(void)
{
#include "../shared/func_8001B530.inc.c"
}

#define SHARED_D0 D_8008F8A0
void func_8006E3B4(Obj80023E04_s80023E04 *obj)
{
#include "../shared/func_80023E04.inc.c"
}

typedef Rect0x8006c2d8 Rect0x8006e3f0;
typedef struct Obj0x8006e3f0 {
    unsigned char pad0[3];
    unsigned char step;
} Obj0x8006e3f0;
extern int func_8007B6EC(Rect0x8006e3f0 *, int, int);
extern void func_8006E450(Obj0x8006e3f0 *);

void func_8006E3F0(Obj0x8006e3f0 *obj)
{
    Rect0x8006e3f0 r;
    r.x = 0x1C0;
    r.y = 0x100;
    r.w = 0x20;
    r.h = 0x100;
    func_8007B6EC(&r, 0x1E0, 0x100);
    obj->step++;
    func_8006E450(obj);
}

