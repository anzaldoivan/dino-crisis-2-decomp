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


typedef struct Work0x800662b8 {
    char pad0[0x364];
    unsigned char x364;
} Work0x800662b8;
#define WORK0x800662b8 (*SCRATCH(Work0x800662b8 *, 0))
typedef struct Sub0x800662b8 {
    unsigned char x0;
    char pad1[5];
    unsigned char x6;
    char pad7;
    unsigned short x8;
} Sub0x800662b8;
typedef struct Obj0x800662b8 {
    char pad0[0x20];
    char x20[0x20];
    unsigned short x40;
    unsigned short x42;
    unsigned short x44;
    char pad46[2];
    char x48[8];
    int x50;
    unsigned char x54;
    unsigned char x55;
    char pad56[0xE0 - 0x56];
    unsigned short xE0;
    unsigned short xE2;
    unsigned short xE4;
    char padE6[0xF0 - 0xE6];
    char xF0[0x174 - 0xF0];
    unsigned short x174;
    char pad176[0x1C0 - 0x176];
    Sub0x800662b8 x1C0;
} Obj0x800662b8;
extern void (*D_8008F7AC[])(Obj0x800662b8 *);
extern int func_80079D34(void *, void *, int);
extern void func_80049E44(Obj0x800662b8 *, void *);
extern void func_8004AC08(Obj0x800662b8 *);
extern void func_80047AA0(Obj0x800662b8 *);

void func_800662B8(Obj0x800662b8 *o)
{
    Sub0x800662b8 *s;
    int t;

    s = &o->x1C0;
    o->xE0 = o->x40;
    o->xE2 = o->x42;
    o->xE4 = o->x44;
    if (o->x174 & 1) {
        o->x174 &= ~1;
        o->x54 = 4;
        o->x55 = 0;
        o->x50 &= ~8;
    }
    D_8008F7AC[o->x54](o);
    t = WORK0x800662b8->x364;
    s->x8++;
    s->x6 = t;
    func_80079D34(o->x48, o->x20, t);
    func_80049E44(o, o->xF0);
    func_8004AC08(o);
    if (!(o->x50 & 2)) {
        if (++o->x1C0.x0 > 0x96) {
            o->x50 |= 0x40000000;
        }
    } else {
        o->x1C0.x0 = 0;
    }
    if (o->x50 & 0x40000000) {
        func_80047AA0(o);
    }
}

typedef struct Sub0x800663ec {
    unsigned char x0;
    unsigned char x1;
    char pad2[1];
    unsigned char x3;
    unsigned char x4;
    unsigned char x5;
    char pad6[2];
    short x8;
    short xA;
} Sub0x800663ec;
typedef struct Obj0x800663ec {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x50 - 0x46];
    int x50;
    unsigned char x54;
    char pad55[0xF4 - 0x55];
    short xF4;
    char padF6[0x108 - 0xF6];
    short x108;
    char pad10A[0x10C - 0x10A];
    unsigned short x10C;
    char pad10E[0x134 - 0x10E];
    void *x134;
    char pad138[0x184 - 0x138];
    int x184;
    char pad188[0x1C0 - 0x188];
    Sub0x800663ec x1C0;
} Obj0x800663ec;
typedef struct Vec0x800663ec {
    short x;
    short y;
    short z;
    short w;
} Vec0x800663ec;
extern int D_8008F798[];
extern unsigned char D_8008F7A4[][2];
extern void func_80047B8C(void *, void *);
extern void func_800484F0(void *, void *, int, int, int);
extern int func_8004A32C(void);

void func_800663EC(Obj0x800663ec *obj) {
    Sub0x800663ec *s;
    Vec0x800663ec v;

    s = &obj->x1C0;
    if (*(signed char *)WORK->padc2a == 0) {
        obj->x10C = 1;
    } else {
        obj->x10C = 2;
    }
    obj->x108 = 0;
    obj->x50 = 0x3000F;
    obj->x134 = D_8008F798;
    func_80047B8C(obj, D_80181500);
    v.y = 0;
    obj->xF4 = D_8008F7A4[obj->x10C][0];
    s->xA = 200;
    obj->x184 |= 2;
    v.x = obj->x40;
    v.z = obj->x44;
    s->x0 = 0;
    s->x1 = func_8004A32C();
    s->x3 = 0x10;
    s->x5 = 0;
    s->x4 = (unsigned int)func_8004A32C() % 90;
    s->x8 = 0;
    func_800484F0(obj, D_801819F4, 0, 1, 0);
    obj->x54 = 1;
}

typedef struct Sub0x8006651c {
    char pad0[4];
    unsigned char x4;
    unsigned char x5;
    char pad6[2];
    unsigned short x8;
    char padA[0x18 - 0xA];
    SVECTOR x18;
} Sub0x8006651c;
typedef struct Obj0x8006651c {
    char pad0[0x40];
    SVECTOR x40;
    char pad48[0x54 - 0x48];
    unsigned char x54;
    char pad55[0xF4 - 0x55];
    unsigned short xF4;
    char padF6[0x10C - 0xF6];
    unsigned short x10C;
    char pad10E[0x17E - 0x10E];
    unsigned short x17E;
    char pad180[0x1C0 - 0x180];
    Sub0x8006651c x1C0;
} Obj0x8006651c;
extern unsigned char D_8008F7A4[][2];
extern int func_800487B8();
extern void func_80044834(void *, int);
extern int func_8004A0F8(SVECTOR *a, SVECTOR *b);
extern void func_80066A8C();

void func_8006651C(Obj0x8006651c *obj)
{
    Obj0x8006651c *u = (Obj0x8006651c *)WORK->units;
    Sub0x8006651c *s = &obj->x1C0;
    unsigned int d;

    func_800487B8();
    if ((unsigned char)++s->x4 % 30 == 0) {
        func_80044834(obj, s->x5 + 10);
        s->x5 = 1 - s->x5;
    }
    func_80066A8C(obj, &u->x40);
    d = func_8004A0F8(&obj->x40, &u->x40);
    if (obj->x10C >= 2) {
        if (d > 4000000) {
            obj->xF4 = D_8008F7A4[obj->x10C][0] + 0x30;
        } else {
            obj->xF4 = D_8008F7A4[obj->x10C][0];
        }
    }
    if (d < 0x57E4 && s->x8 >= 0x3D) {
        obj->x54 = 2;
        s->x8 = 0;
    }
    if (u->x17E == 0) {
        obj->x54 = 3;
        obj->xF4 = (func_8004A32C() & 0x1F) | 0x20;
        obj->x10C = func_8004A32C() & 1;
        s->x18 = u->x40;
    }
}

typedef struct Sub0x800666c8 {
    char pad0[8];
    unsigned short x8;
} Sub0x800666c8;
typedef struct Obj0x800666c8 {
    char pad0[0x40];
    unsigned short x40;
    unsigned short x42;
    unsigned short x44;
    char pad46[0x4A - 0x46];
    unsigned short x4A;
    char pad4c[0x50 - 0x4C];
    int x50;
    unsigned char x54;
    unsigned char x55;
    unsigned char x56;
    char pad57[0x176 - 0x57];
    unsigned short x176;
    unsigned short x178;
    char pad17a[0x1C0 - 0x17A];
    Sub0x800666c8 x1C0;
} Obj0x800666c8;
extern void func_80044AB4();

void func_800666C8(Obj0x800666c8 *obj)
{
    Obj0x800666c8 *u = (Obj0x800666c8 *)WORK->units;
    Sub0x800666c8 *s = &obj->x1C0;

    if (s->x8 == 1) {
        obj->x50 &= ~0xE;
        func_80044AB4(10, 1);
        return;
    }
    if (s->x8 < 0x1F && u->x54 == 1) {
        return;
    }
    if (u->x54 == 1 && u->x55 < 8) {
        u->x178 = 0;
        u->x176 = 0;
        u->x54 = 2;
        u->x55 = 0;
        u->x56 = 0;
    }
    obj->x40 = u->x40;
    obj->x42 = u->x42 - 0x578;
    obj->x44 = u->x44;
    obj->x4A = func_8004A32C();
    obj->x54 = 1;
    obj->x50 |= 0xE;
    s->x8 = 0;
}

typedef struct Sub0x800667b8 {
    char pad0[4];
    unsigned char x4;
    unsigned char x5;
    unsigned char x6;
    char pad7[0x18 - 0x7];
    char x18[1];
} Sub0x800667b8;
typedef struct Obj0x800667b8 {
    char pad0[0x50];
    int x50;
    char pad54[0x1C0 - 0x54];
    Sub0x800667b8 x1C0;
} Obj0x800667b8;
extern int func_800487B8();
extern void func_80044834(void *, int);
extern void func_80066A8C();

void func_800667B8(Obj0x800667b8 *obj)
{
    Sub0x800667b8 *s = &obj->x1C0;

    func_800487B8();
    if ((unsigned char)++s->x4 % 90 == 0) {
        func_80044834(obj, s->x5 + 10);
        s->x5 = 1 - s->x5;
    }
    func_80066A8C(obj, &obj->x1C0.x18);
    if (s->x6 != (unsigned char)WORK->x318[0x364 - 0x318]) {
        obj->x50 |= 0x40000000;
    }
}

typedef struct Sub0x80066888 {
    unsigned char pad0[0xA];
    short xA;
    short xC;
    unsigned char padE[2];
    unsigned short x10;
} Sub0x80066888;
typedef struct Obj0x80066888 {
    unsigned char pad0[0x40];
    unsigned char x40[0x10];
    int x50;
    unsigned char pad54;
    unsigned char x55;
    unsigned char pad56[0x64 - 0x56];
    int x64;
    unsigned char pad68[0xF0 - 0x68];
    short xF0;
    short xF2;
    short xF4;
    unsigned char padF6[0x1C0 - 0xF6];
    Sub0x80066888 x1C0;
    unsigned char pad1D2[0x1E4 - 0x1D2];
    signed char x1E4[6];
} Obj0x80066888;
typedef struct Part0x80066888 {
    unsigned char pad0[0x48];
    short x48;
    unsigned char pad4A[0x58 - 0x4A];
    void *x58;
    unsigned char pad5C[0x64 - 0x5C];
    short x64;
    short x66;
    short x68;
} Part0x80066888;
typedef struct Tbl0x80066888 {
    signed char div;
    signed char add;
    unsigned short ang;
} Tbl0x80066888;
extern Tbl0x80066888 D_8008F7C0[];
extern Part0x80066888 *func_800474B4(int a, int b);
extern int func_8004A2C4();
extern int func_8004A32C(void);
extern int func_800789B4(int a);
extern int func_80078C44(int a);

void func_80066888(Obj0x80066888 *obj)
{
    int one;
    Part0x80066888 *p;
    unsigned int i;
    Sub0x80066888 *sub;
    void *units;

    units = WORK->units;
    sub = &obj->x1C0;
    switch (obj->x55) {
    case 0:
        obj->x55 = one = 1;
        sub->x10 = func_8004A2C4(obj->x40, (char *)units + 0x40);
        sub->xC = 5;
        sub->xA = one;
        obj->xF4 = 0;
        obj->xF2 = 0;
        obj->xF0 = 0;
        for (i = 0; i < 6; i++) {
            func_800474B4(obj->x64, i)->x58 = obj;
            obj->x1E4[i] = func_8004A32C();
        }
        break;
    case 1:
        if (sub->xC != 0) {
        sub->xC--;
        for (i = 0; i < 6; i++) {
            int a;
            p = func_800474B4(obj->x64, i);
            a = (sub->x10 + D_8008F7C0[i].ang + obj->x1E4[i]) & 0xFFF;
            p->x64 += func_80078C44(a) / D_8008F7C0[i].div;
            p->x68 += func_800789B4(a) / D_8008F7C0[i].div;
            p->x66 += D_8008F7C0[i].add;
            p->x48 += 0x28;
        }
        sub->xA += 2;
        } else {
            obj->x50 |= 0x40000000;
        }
        break;
    }
}

typedef struct Obj0x80066a8c {
    unsigned char pad0[0x40];
    short x40;
    unsigned short x42;
    unsigned char pad44[0x4A - 0x44];
    short x4A;
    unsigned char pad4C[0x10C - 0x4C];
    unsigned short x10C;
    unsigned char pad10E[0x1C0 - 0x10E];
    Sub0x80066a8c x1C0;
} Obj0x80066a8c;
extern int func_80078844(int);
extern short func_80066B80(int a, int b, short ang, short step);

void func_80066A8C(Obj0x80066a8c *obj, SVECTOR *b)
{
    Sub0x80066a8c *s = &obj->x1C0;
    int y;

    s->x1 += s->x3;
    y = obj->x42 + (func_80078844(s->x1 << 4) >> 7);
    obj->x42 = y;
    if (b->vy - 100 < (short)y) {
        obj->x42 = y - 10;
    } else if ((short)y < b->vy - 2000) {
        obj->x42 = y + 10;
    }
    if ((func_8004A32C() & 0xF) == 0) {
        s->x3 = (func_8004A32C() & 7) + 4;
    }
    obj->x4A = (obj->x4A + func_80066B80((int)&obj->x40, (int)b, obj->x4A, D_8008F7A4[obj->x10C][1])) & 0xFFF;
}

#define SHARED_F0 func_8004A2C4
short func_80066B80(int a, int b, short ang, short step)
{
#include "../shared/func_80066B80.inc.c"
}

extern void func_80034C48(int, int, int);
extern void (*D_8008F7F0[])(Obj80066C20 *);

void func_80066C20(Obj80066C20 *obj)
{
    func_80034C48(3, 0, 0);
    D_8008F7F0[obj->state](obj);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_80066C74);

typedef struct S0x80066e20 {
    char pad0[8];
    short x8;
    char padA[0x26];
    short x30;
} S0x80066e20;
struct S0x800689f4;
extern struct S0x800689f4 D_800B53C8;
extern int func_8004A32C(void);
extern void func_80069890(short, unsigned short, unsigned short);
extern void func_8006BD80();

void func_80066E20(void *arg0)
{
    int i;
    int a;
    int b;
    int c;

    if (((S0x80066e20 *)&D_800B53C8)->x8 != ((S0x80066e20 *)&D_800B53C8)->x30) {
        if ((unsigned char)((unsigned int)func_8004A32C() % 3) != 0) {
            for (i = 0; i < (func_8004A32C() & 7) + 1; i++) {
                a = (unsigned char)((unsigned int)func_8004A32C() % 239);
                b = (unsigned char)((unsigned int)func_8004A32C() % 15) - 8;
                c = func_8004A32C() & 1;
                func_80069890(b, a, ++c);
            }
        }
    }
    func_8006BD80(arg0, 0x80, ((S0x80066e20 *)&D_800B53C8)->x8 >= 0x55);
    ((S0x80066e20 *)&D_800B53C8)->x30 = ((S0x80066e20 *)&D_800B53C8)->x8;
}

typedef struct Obj0x80066f78 {
    unsigned char pad0[3];
    unsigned char x3;
    void *x4;
} Obj0x80066f78;
extern void func_80067010();

void func_80066F78(Obj0x80066f78 *obj)
{
    S0x80066e20 *p;

    func_80034C48(1, 3, 1);
    p = (S0x80066e20 *)&D_800B53C8;
    p->x8++;
    if (p->x8 > 30000) {
        p->x8 = 30000;
    }
    if (p->x8 > 0x5A) {
        p->x8 = 0;
        obj->x3++;
        func_80067010(obj);
    }
    obj->x4 = func_80066E20;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_80067010);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_80067B4C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_80067F2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_8006837C);

typedef struct S0x800689f4 {
    char pad0[4];
    short x4;
} S0x800689f4;
extern S0x800689f4 D_800B53C8;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
extern void func_8007E194();

void func_800689F4(void)
{
    PolyF4_0x800689f4 p;
    char *s;
    unsigned int *ot;
    unsigned int *prim;

    s = WORK->x318;
    if (D_800B53C8.x4 == 0) {
        return;
    }
    if (D_800B53C8.x4 >= 0x27) {
        if (*(short *)(s + 0x20) < 0x800) {
            if (*(short *)(s + 0x20) >= 0xC9) {
                *(short *)(s + 0x20) = 0xC8;
            }
        } else if (*(short *)(s + 0x20) < 0xF7E) {
            *(short *)(s + 0x20) = 0xF7E;
        }
        if (*(short *)(s + 0x22) < 0x800) {
            if (*(short *)(s + 0x22) >= 0x317) {
                *(short *)(s + 0x22) = 0x316;
            }
        } else if (*(short *)(s + 0x22) < 0xCEA) {
            *(short *)(s + 0x22) = 0xCEA;
        }
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
    p.g0 = p.b0 = D_800B53C8.x4;
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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_80068CA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_800694A0);

extern unsigned char D_800ABA96;
extern unsigned int D_800AC074[][0x400];
extern void func_8007E074();
extern void func_8007E134();

void func_80069890(short n, unsigned short y, unsigned short h)
{
    Box0x8002243c r0;
    Box0x8002243c r1;
    int yy;

    if (n == 0) {
        return;
    }
    if (n > 0) {
        r0.x = 0;
        r0.y = y;
        r0.w = 320 - n;
        r0.h = h;
        if (D_800ABA96) {
            yy = y;
            yy += 0x100;
        } else {
            yy = y;
        }
        r0.y = yy;
        func_8007E134(PKT, &r0, n, r0.y);
    } else {
        r1.x = 0;
        r1.y = y;
        r1.w = n + 320;
        r1.h = h;
        if (D_800ABA96) {
            yy = y;
            yy += 0x100;
        } else {
            yy = y;
        }
        r1.y = yy;
        func_8007E134(PKT, &r1, 0, r1.y);
    }
    func_8007E074(D_800AC074[D_800ABA96], PKT);
    PKT += 6;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_800699A8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800662B8", func_80069C00);

extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern void func_80034C48(int, int, int);
extern void (*D_8008F894[])(Obj0x8006c538 *);



#define SHARED_D0 D_8008F8A0

