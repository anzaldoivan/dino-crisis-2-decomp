#include "common.h"
#include "dc2.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;



typedef struct Obj0x80039978 {
    char pad0[0x55];
    unsigned char mode;
    unsigned char x56;
} Obj0x80039978;
extern void func_80034C48(int, int, int);
void func_8003EA64(Obj0x80039978 *);
extern void (*D_80089490[])(Obj0x80039978 *);

void func_80039978(Obj0x80039978 *p)
{
    if (p->x56 == 0) {
        func_8003EA64(p);
    }
    if (WORK->xC2B == 2) {
        func_80034C48(1, 9, 0);
    }
    D_80089490[p->mode](p);
}

typedef struct Obj0x80039a00 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x55 - 0x46];
    unsigned char mode;
    char pad56[0x5A - 0x56];
    short x5A;
    char pad5c[0xF0 - 0x5C];
    int xF0;
    short xF4;
} Obj0x80039a00;
typedef struct {
    short x;
    short y;
    short z;
    short pad;
} Vec0x80039a00;
extern void func_800484F0();
extern int func_800487B8();
extern void func_80030DDC();
extern short func_80033F10();

void func_80039A00(Obj0x80039a00 *p)
{
    Vec0x80039a00 v;

    switch (p->mode) {
    case 0:
        func_800484F0(p, 8, 0, 0, 0);
        p->xF0 = 0;
        p->xF4 = 0;
        p->mode++;
        func_80030DDC(0);
        break;
    case 1:
        func_800487B8(p);
        if (WORK->xC2B == 2) {
            v.y = 10000;
            v.x = p->x40;
            v.z = p->x44;
            p->x5A = func_80033F10(&v, &p->x40, 0x80, 0) / -1000;
            if (p->x42 < p->x5A * -1000) {
                p->x42 += 100;
            }
            if (p->x5A * -1000 < p->x42) {
                p->x42 = p->x5A * -1000;
            }
            p->x5A = p->x42 / -1000;
        }
        break;
    }
}

typedef struct Cam0x80039b7c {
    char pad0[0x58];
    short x58;
    short x5A;
    char pad5c[0x60 - 0x5C];
    short x60;
    short x62;
} Cam0x80039b7c;
typedef struct Obj0x80039b7c Obj0x80039b7c;
extern Cam0x80039b7c *D_800B25DC;
extern void func_80034C48(int, int, int);
void func_8003EA64(Obj0x80039978 *);

void func_80039B7C(Obj0x80039b7c *p)
{
    if ((WORK->xC50 & 0xF) == 6) {
        D_800B25DC->x60 = 0;
        D_800B25DC->x62 = 0;
        D_800B25DC->x58 = 0;
        D_800B25DC->x5A = 0;
    }
    func_8003EA64(p);
    if (WORK->xC2B == 2) {
        func_80034C48(1, 9, 0);
    }
    WORK->x4CC(p);
}

void func_80039C20(void)
{
    WORK->x4D0();
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_80039C54);

#define SPAD_080x8003a160 (*SCRATCH(unsigned short, 0x8))
#define SPAD_0A0x8003a160 (*SCRATCH(unsigned short, 0xA))
typedef struct Obj0x8003a160 {
    char pad0[0x54];
    union {
        int w;
        unsigned char b[4];
    } x54;
    char pad58[0xF0 - 0x58];
    int xF0;
    short xF4;
    char padf6[0x11A - 0xF6];
    unsigned char x11A;
    unsigned char x11B;
    char pad11c[0x174 - 0x11C];
    unsigned short x174;
    char pad176[0x1DC - 0x176];
    int x1DC;
} Obj0x8003a160;
extern Cam0x80039b7c *D_800B25DC;
extern void func_80034C48(int, int, int);
extern int func_8003D2D4(Obj0x8003a160 *, int);
extern int func_8003D58C(Obj0x8003a160 *, int, int, int);
extern int func_80040F7C(void);
extern void func_800484F0(Obj0x8003a160 *, int, int, int, int);
extern int func_800487B8(Obj0x8003a160 *);

void func_8003A160(Obj0x8003a160 *p)
{
    unsigned short pad;

    switch (p->x54.b[2]) {
    case 0:
        func_800484F0(p, 0x16, ((unsigned short *)D_800B25DC)[0x1E / 2], 1,
                      ((unsigned short *)D_800B25DC)[0x1C / 2]);
        p->xF0 = 0;
        p->xF4 = 0;
        p->x54.b[2]++;
        break;
    case 1:
        func_800487B8(p);
        break;
    }
    if (SPAD_080x8003a160 & 0xA0) {
        if (func_8003D2D4(p, 0x280) && func_8003D58C(p, 1000, 200, (p->x11A & 0x30) << 6)) {
            pad = p->x174;
            p->x1DC = 5;
            p->x11B = 0;
            p->x54.w = 0x801;
            p->x174 = pad | 1;
            return;
        }
        func_80034C48(2, 13, 1);
    }
    if (SPAD_0A0x8003a160 & 0x40) {
        func_80034C48(2, 13, 0);
        func_80034C48(1, 9, 1);
        p->x11B = 0;
        p->x1DC = 4;
        p->x54.w = 0x901;
        return;
    }
    if ((SPAD_0A0x8003a160 & 0x10) && (WORK->xC50 >> 8)) {
        if (func_80040F7C()) {
            return;
        }
        func_80034C48(2, 13, 0);
        p->x54.w = 0xA01;
        return;
    }
    pad = SPAD_0A0x8003a160;
    if (pad & 0xA0) {
        func_80034C48(2, 1, 1);
        return;
    }
    if (pad & 2) {
        func_80034C48(2, 13, 0);
        p->x11B = 0;
        p->x1DC = 4;
        p->x54.w = 0x701;
        return;
    }
    if ((SPAD_080x8003a160 & 8) && (WORK->xC50 & 0xF)) {
        func_80034C48(2, 13, 0);
        p->x54.w = 0x501;
        return;
    }
    pad = SPAD_080x8003a160;
    if (pad & 0x1000) {
        func_80034C48(2, 13, 0);
        p->x11B = 0;
        p->x1DC = 4;
        p->x54.w = 0x201;
    } else if (pad & 0x4000) {
        func_80034C48(2, 13, 0);
        p->x1DC = 8;
        p->x54.w = 0x401;
    } else if (pad & 0xA000) {
        func_80034C48(2, 13, 0);
        p->x1DC = 4;
        p->x54.w = 0x301;
    }
}

void func_8003A410(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003A418);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003A954);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003AC70);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003B08C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003B494);

typedef struct Obj0x8003b58c {
    char pad0[0x42];
    short f42;
    char pad44[0x4A - 0x44];
    short f4A;
    char pad4c[0x50 - 0x4C];
    int f50;
    union {
        int w;
        unsigned char b[4];
    } f54;
    char pad58[0x5A - 0x58];
    short f5A;
    char pad5c[0xF0 - 0x5C];
    int fF0;
    short fF4;
    char padf6[0x11A - 0xF6];
    unsigned char f11A;
    unsigned char f11B;
    char pad11c[0x174 - 0x11C];
    unsigned short f174;
    char pad176[0x1DC - 0x176];
    int f1DC;
} Obj0x8003b58c;
extern struct Cam0x80039b7c *D_800B25DC;
extern void func_800484F0();
extern int func_800487B8();
extern int func_8003DD34(int, int, unsigned short);
extern void func_8004A4D4();
extern void func_8004A4E8();

void func_8003B58C(Obj0x8003b58c *o)
{
    int v;

    switch (o->f54.b[2]) {
    case 0:
        func_800484F0((void *)o, 0x1A, 0, 1, 3);
        o->fF0 = 0;
        o->fF4 = 0;
        o->f11B = 0;
        o->f174 |= 1;
        o->f50 |= 0x800;
        ((unsigned short *)D_800B25DC)[0x20 / 2] = (o->f11A & 0x30) << 6;
        o->f54.b[2]++;
        break;
    case 1:
        func_800487B8((void *)o);
        v = func_8003DD34(o->f4A, 0x40, ((unsigned short *)D_800B25DC)[0x20 / 2]);
        o->f4A += v;
        if (v == 0) {
            if (o->f11A & 0x40) {
                o->f5A -= 2;
                func_800484F0((void *)o, 0x14, 0, 0x40, 0);
            } else {
                o->f5A += 2;
                func_800484F0((void *)o, 0x13, 0, 0x40, 0);
            }
            o->f50 |= 0x30;
            func_8004A4E8((void *)o);
            o->f54.b[2]++;
        }
        break;
    case 2:
        if (func_800487B8((void *)o)) {
            o->f42 = -(o->f5A * 1000);
            o->f174 &= ~1;
            o->f50 &= ~0x20;
            o->f50 &= ~0x10;
            o->f50 &= ~0x800;
            func_8004A4D4((void *)o, 8, 0x1000, 0x1000);
            o->f1DC = 4;
            o->f54.w = 1;
        }
        break;
    }
}

typedef struct {
    char pad0[0xC50];
    unsigned short xC50;
} Wk0x8003b778;
#define WORK0x8003b778 (*SCRATCH(Wk0x8003b778 *, 0))
#define SPAD_0A0x8003b778 (*SCRATCH(unsigned short, 0xA))
typedef struct Src0x8003b778 {
    char pad0[0x20];
    char x20[0x20];
    short x40;
    short x42;
    short x44;
} Src0x8003b778;
typedef struct Eff0x8003b778 {
    char pad0[0x20];
    char x20[0x20];
    short x40;
    short x42;
    short x44;
    short pad46;
    short x48;
    short x4A;
    short x4C;
    unsigned char x4E;
    unsigned char x4F;
    int x50;
    int x54;
    int x58;
    short x5C;
    short pad5E;
    short x60;
    short x62;
    short x64;
    short pad66;
    void *x68;
} Eff0x8003b778;
typedef struct Obj0x8003b778 {
    char pad0[0x4A];
    short x4A;
    char pad4c[0x64 - 0x4C];
    int x64;
} Obj0x8003b778;
extern Cam0x80039b7c *D_800B25DC;
extern int func_80052364();
extern void func_80044834();
extern void func_8004AC08();
extern char *func_800474B4();
extern int func_8003F154();
extern void func_8001C5AC();
extern void func_8001C614();
extern void func_80047E0C();
extern void func_8005247C();
extern void *func_800402EC();
extern SVECTOR *func_80079704();
extern void func_8004A240();
extern void func_8007AA34();
extern void func_80048048();

int func_8003B778(Obj0x8003b778 *p)
{
    SVECTOR v;
    Src0x8003b778 *s;
    Eff0x8003b778 *e;

    ((short *)D_800B25DC)[0x3C / 2] = 0;
    ((char *)D_800B25DC)[0x2F] = 0;
    if (func_80052364(1) == 0) {
        if (SPAD_0A0x8003b778 & 0xA0) {
            func_80044834(p, 0x11);
        }
        return 1;
    }
    func_8004AC08(p);
    if ((WORK0x8003b778->xC50 & 0xF) == 10) {
        s = (Src0x8003b778 *)func_800474B4(p->x64, 3);
        func_8003F154(s->x20, &s->x40, 10);
        func_8001C5AC(p, 5);
        func_8001C614(p, 5, 0x3C);
        func_80047E0C(p, D_80177920);
        func_8005247C(0);
        func_80044834(p, 0x10);
        return 0;
    }
    e = func_800402EC();
    if (e != 0) {
        s = (Src0x8003b778 *)func_800474B4(p->x64, 3);
        v.vx = 0;
        v.vy = 0xA28;
        v.vz = 0;
        func_80079704(s->x20, &v, &v);
        v.vx += s->x40;
        v.vy += s->x42;
        v.vz += s->x44;
        func_8004A240(&s->x40, &v, &v);
        e->x68 = D_801789C4;
        e->x50 = 3;
        e->x54 = 0;
        e->x4E = 3;
        e->x4F = 7;
        e->x48 = 0x400 - v.vx;
        e->x4A = p->x4A;
        e->x4C = 0;
        func_8007AA34(&e->x48, e->x20);
        e->x40 = s->x40;
        e->x42 = s->x42 + 0x1E;
        e->x44 = s->x44;
        e->x58 = *(int *)&e->x40;
        e->x5C = e->x44;
        e->x60 = 0;
        e->x62 = 500;
        e->x64 = 0;
        func_80047E0C(p, D_80178B88);
        func_80048048(e, D_80178D48, 0, 0, 10, 0);
        func_8005247C(1);
        func_8001C5AC(p, 8);
        func_8001C614(p, 8, 0x5A);
        func_80044834(p, 0x10);
    }
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003B9F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003C498);

typedef struct Wk0x8003c688 {
    char pad0[0x214];
    unsigned short x214;
    char pad216[0x4C7 - 0x216];
    unsigned char x4C7;
    char pad4c8[0xC2B - 0x4C8];
    unsigned char xC2B;
    char padc2c[0xC50 - 0xC2C];
    unsigned short xC50;
} Wk0x8003c688;
#define W0x8003c688 (*SCRATCH(Wk0x8003c688 *, 0))
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);
extern unsigned short D_8001878C[];
extern int (**D_800AE074)(Obj0x80044834 *);

void func_8003C688(Obj0x80044834 *obj)
{
    unsigned short idx = 0;
    unsigned char v;

    switch (W0x8003c688->xC2B) {
    case 3:
        obj->x58 = 0x2B;
        W0x8003c688->xC50 = 12;
        break;
    case 4:
        idx = 1;
        obj->x58 = 0x14;
        W0x8003c688->xC50 = 13;
        break;
    default:
        if (W0x8003c688->xC2B >= 7) {
            v = W0x8003c688->xC2B;
            obj->x58 = v + 14;
            idx = v - 5;
            W0x8003c688->xC50 = W0x8003c688->xC2B + 7;
            break;
        }
        if (W0x8003c688->x4C7 != 0) {
            switch (W0x8003c688->xC2B) {
            case 0:
                W0x8003c688->xC50 = 6;
                break;
            case 1:
                W0x8003c688->xC50 = 0x101;
                break;
            case 5:
                W0x8003c688->xC50 = 0x604;
                break;
            case 6:
                W0x8003c688->xC50 = 5;
                break;
            }
        }
        D_800AE074[obj->x58](obj);
        return;
    }
    if (W0x8003c688->x214 != D_8001878C[idx]) {
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        func_8001EC10(D_8001878C[idx]);
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        W0x8003c688->x214 = D_8001878C[idx];
    }
    D_800AE074[obj->x58](obj);
}

extern Work D_800AE080;

void func_8003C8D8(Unit *self) {
    Unit *u = WORK->units;
    unsigned int i;

    self->flags = 1;
    for (i = 1; i < D_800AF11C; i++) {
        u++;
        if ((u->flags & 1) && u->x1C0) {
            u->x1C0 = 0;
            u->x1C2 += 4;
            u->x54 = (u->x1C2 << 8) | 2;
            if (u == D_800AE080.units + 1) {
                func_80034C48(7, 0x1C, 1);
            } else {
                func_80034C48(7, 0x1D, 1);
            }
        }
    }
}

extern int func_80034CD8(int, int);

unsigned short func_8003C9D0(unsigned short arg0) {
    if ((signed char)WORK->padc2a[0] == 2) {
        if (func_80034CD8(2, 0x26)) {
            arg0 = arg0 * 2;
        } else if (func_80034CD8(2, 0x27)) {
            arg0 = (arg0 * 5) / 2;
        } else {
            arg0 = arg0 * 3;
        }
    }
    if ((signed char)WORK->padc2a[0] == 0) {
        arg0 = (arg0 * 2) / 3;
    }
    if (func_80034CD8(9, 0x33)) {
        arg0 >>= 1;
    }
    if (WORK->x4C7 == 1 && (WORK->xC2B >= 7 || WORK->xC2B == 4)) {
        arg0 >>= 1;
    }
    return arg0;
}

typedef struct Unit0x8003cadc {
    char pad0[0x17E];
    unsigned short x17E;
} Unit0x8003cadc;
typedef struct Work0x8003cadc {
    char pad0[0x4E0];
    Unit0x8003cadc *units;
    char pad4e4[0xC2B - 0x4E4];
    unsigned char xC2B;
} Work0x8003cadc;
#define WORK0x8003cadc (*SCRATCH(Work0x8003cadc *, 0))
int func_8004A32C(void);

int func_8003CADC(unsigned short a0)
{
    Unit0x8003cadc *u;

    u = WORK0x8003cadc->units;
    if (WORK0x8003cadc->xC2B < 3) {
        if (u->x17E) {
            u->x17E += a0 >> 1;
        }
        if ((unsigned int)func_8004A32C() < 0x81) {
            // !FAKE: goto from the w39 match; keeps the target's block order (store tail after return 0)
            goto store;
        }
    }
    return 0;
store:
    u->x17E = a0 + 700;
    return a0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003CB5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80039978", func_8003CFC0);

typedef struct {
    char pad0[0x4A];
    short x4A;
    char pad4c[0x178 - 0x4C];
    short x178;
} Obj0x8003d1e0;
extern unsigned char D_8001879C[];

unsigned char func_8003D1E0(Obj0x8003d1e0 *o) {
    int b;
    int a;
    unsigned char *p;

    b = ((o->x178 + 0xA00) >> 10) & 3;
    a = (((o->x4A + 0x200) >> 10) & 3) + 3;
    b = a - b;
    p = &D_8001879C[b];
    o->x178 = *p;
    return *p;
}

extern unsigned char D_800187A4[];

unsigned char func_8003D22C(Unit *a)
{
    Unit *t = WORK->units;
    unsigned char *p;
    int d;

    if (a->x184 & 0x40) {
        t = WORK->x6F0;
    }
    d = ((t->x48.vy + 0x200) >> 10) & 3;
    d = 3 - d + (((a->x48.vy + 0x200) >> 10) & 3);
    p = &D_800187A4[d];
    t->x178 = *p;
    return *p;
}

extern u16 D_800AF132;

s32 func_8003D29C(void) {
    s32 var_a1;
    u16 var_v1;
    u32 var_a0;

    var_v1 = D_800AF132;
    var_a0 = 0;
    var_a1 = 0;
    do {
        var_a1 += var_v1 & 1;
        var_a0 += 1;
        var_v1 = (u16) (var_v1 >> 1);
    } while (var_a0 < 0x10U);
    return var_a1 * 0xA;
}


extern Elem28 D_800B4A68[];


