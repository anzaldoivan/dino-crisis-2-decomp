#include "common.h"
#include "dc2.h"
#include "../shared/func_800436C8.h"
#include "../shared/func_8002B3E0.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;



INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003DD34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003DD90);

unsigned short func_8003DE8C(short a, short b, short c, short d)
{
    if (c & 0x800) {
        c |= 0xF000;
    }
    if (d < (c < 0 ? -c : c)) {
        if (c & 0x800) {
            c = (c - a) | 0xF000;
            if (c < (short)-b) {
                c = -b;
            }
        } else {
            c = (c - a) & 0xFFF;
            if (b < c) {
                c = b;
            }
        }
    } else {
        c = (c - a) & 0xFFF;
        if (c & 0x800) {
            c |= 0xF000;
            if (c < (short)-b) {
                c = -b;
            }
        } else if (b < c) {
            c = b;
        }
    }
    return c;
}

extern int func_8004A0F8(void *a, void *b);
extern int func_8004A2C4(void *p, void *arg);

int func_8003DF80(char *obj, void *target, unsigned int dist, unsigned int range)
{
    char *pos = obj + 0x40;
    unsigned int d;

    if (dist < (unsigned int)func_8004A0F8(pos, target)) {
        return 0;
    }
    d = func_8004A2C4(pos, target) - *(short *)(obj + 0x4A);
    d &= 0xFFF;
    if (range < d && d < 0x1000 - range) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E020);

typedef struct Obj0x8003e144 {
    char pad0[0x140];
    int f140;
    int f144;
    char pad148[0x150 - 0x148];
    int f150;
    int f154;
    int f158;
    char pad15C[0x168 - 0x15C];
    short f168;
    short pad16A;
    short f16C;
} Obj0x8003e144;

void func_8003E144(Obj0x8003e144 *a0)
{
    a0->f150 = 0;
    a0->f154 = 0;
    a0->f140 = 0;
    a0->f144 = 0;
    a0->f168 = 0;
    a0->f16C = 0;
    a0->f158 = 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E164);

typedef struct Obj0x8003e1b4 {
    char pad0[0x158];
    signed char mode;
    char pad159[0x15E - 0x159];
    unsigned char x15E;
} Obj0x8003e1b4;
extern unsigned char D_800AE547;
typedef void (*Fn0x8003e1b4)(Obj0x8003e1b4 *);
extern Fn0x8003e1b4 D_800894A0[];

void func_8003E1B4(Obj0x8003e1b4 *p)
{
    int v;

    if (D_800AE547 != 2) {
        v = WORK->xC2B;
        if (v < 5) {
            if (v >= 2) {
                p->x15E = 0;
            }
        }
    }
    D_800894A0[p->mode](p);
}

void func_8003E230(void)
{
#include "../shared/func_8001B530.inc.c"
}

typedef struct Obj0x8003e238 {
    char pad0[0x64];
    int f64;
    char pad68[0x140 - 0x68];
    short f140;
    short f142;
    char pad144[0x15F - 0x144];
    unsigned char f15F;
    char pad160;
    unsigned char f161;
    char pad162[2];
    short f164;
    short f166;
    char pad168[2];
    unsigned short f16A;
    char pad16C[2];
    unsigned short f16E;
} Obj0x8003e238;
typedef struct Ent0x8003e238 {
    char pad0[0x48];
    short f48;
    short f4A;
} Ent0x8003e238;
char *func_800474B4(int a, int b);
int func_8003DD34(int, int, unsigned short);

void func_8003E238(Obj0x8003e238 *o)
{
    Ent0x8003e238 *e;

    e = (Ent0x8003e238 *)func_800474B4(o->f64, o->f15F);
    if (o->f16A != 0 && !(o->f161 & 1)) {
        o->f140 = (o->f140 + func_8003DD34(o->f140, o->f164, e->f48)) & 0xFFF;
        if (e->f48 == o->f140) {
            o->f161 |= 1;
        }
        e->f48 = o->f140;
    } else {
        o->f140 = e->f48;
    }
    if (o->f16E != 0 && !(o->f161 & 2)) {
        o->f142 = (o->f142 + func_8003DD34(o->f142, o->f166, e->f4A)) & 0xFFF;
        if (e->f4A == o->f142) {
            o->f161 |= 2;
        }
        e->f4A = o->f142;
    } else {
        o->f142 = e->f4A;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E378);

typedef struct Obj0x8003e488 {
    char pad00[0x64];
    int id;
    char pad68[0x150 - 0x68];
    char pos[0xF];
    unsigned char part;
    char pad160[0x168 - 0x160];
    short x;
    short pad16A;
    short y;
} Obj0x8003e488;
char *func_800474B4(int a, int b);
void func_8004A240(void *, void *, SVECTOR *);
extern void func_8003E4E0(Obj0x8003e488 *);

void func_8003E488(Obj0x8003e488 *obj)
{
    SVECTOR v;
    func_8004A240(obj->pos, func_800474B4(obj->id, obj->part) + 0x40, &v);
    obj->x = v.vx;
    obj->y = v.vy;
    func_8003E4E0(obj);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E4E0);

void func_8003E63C(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E644);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E6E4);

#define SHARED_D0 D_800894C8
void func_8003E740(Obj8002B3E0_s8002B3E0 *obj)
{
#include "../shared/func_8002B3E0.inc.c"
}

#define SHARED_F0 func_800487B8
void func_8003E77C(void)
{
#include "../shared/func_8001EA54.inc.c"
}

typedef struct Obj0x8003e79c {
    unsigned char pad0[0x55];
    unsigned char f55;
    unsigned char pad56[0xF0 - 0x56];
    short fF0;
    short fF2;
    short fF4;
} Obj0x8003e79c;

void func_8003E79C(Obj0x8003e79c *p) {
    p->fF4 = 0;
    p->fF2 = 0;
    p->fF0 = 0;
    p->f55 = 0;
}

extern void (*D_800899E0[])(void *);
extern void (*D_80089A30[])(void *);

void func_8003E7B0(void *arg0)
{
    D_800899E0[*(unsigned char *)&WORK->xC50](arg0);
    D_80089A30[WORK->xC50 >> 8](arg0);
}

extern void (*D_80089A58[])(void *);

void func_8003E830(void *arg0)
{
    D_80089A58[WORK->xC50 & 0xF](arg0);
}

typedef struct Sub0x8003e878 {
    unsigned char pad0[0xA];
    unsigned char xA;
} Sub0x8003e878;
typedef struct Ent0x8003e878 {
    unsigned char pad0[0x64];
    int x64;
    unsigned char pad68[0x134 - 0x68];
    Sub0x8003e878 *x134;
} Ent0x8003e878;
typedef struct Glob0x8003e878 {
    unsigned char pad0[0x14];
    Ent0x8003e878 *x14;
    unsigned char pad18[0x30 - 0x18];
    short x30;
} Glob0x8003e878;
typedef struct Obj0x8003e878 {
    unsigned char pad0[0x40];
    short x40;
    short x42;
    short x44;
    short x46;
    SVECTOR x48;
} Obj0x8003e878;
extern struct Glob0x8004084c *D_800B25DC;
extern void (*D_80089A88[])(void *);
extern void func_8003FE00();
extern int func_800412C8();
char *func_800474B4(int a, int b);
void func_8004A240(void *, void *, SVECTOR *);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007A824(SVECTOR *r, MATRIX *m);
#define G0x8003e878 ((Glob0x8003e878 *)D_800B25DC)

void func_8003E878(Obj0x8003e878 *p)
{
    Ent0x8003e878 *ent;
    char *s;
    unsigned short ang;
    unsigned short u;
    SVECTOR out;
    SVECTOR v;
    MATRIX m;

    func_8003FE00(p, 0x200);
    ent = G0x8003e878->x14;
    if (ent != 0) {
        s = func_800474B4(ent->x64, ent->x134->xA);
        v.vx = 0x16;
        v.vy = 0;
        v.vz = -0xDE;
        func_8007A824(&p->x48, &m);
        func_80079704(&m, &v, &v);
        v.vx += p->x40;
        v.vy += p->x42 - 1000;
        v.vz += p->x44;
        func_8004A240(s + 0x40, &v, &out);
        u = out.vx;
        if (out.vx >= 0) {
            if (out.vx > 0x200) {
                ang = 0x1000;
            } else {
                ang = u * 4 + 0x800;
            }
        } else if (out.vx < -0x200) {
            ang = 0;
        } else {
            ang = (u + 0x200) * 4;
        }
    } else {
        ang = 0x800;
    }
    G0x8003e878->x30 += func_800412C8(G0x8003e878->x30, 0x180, ang);
    D_80089A88[WORK->xC50 & 0xF](p);
}

extern void (*D_80089AB8[])(void *);

void func_8003EA1C(void *arg0)
{
    D_80089AB8[WORK->xC50 >> 8](arg0);
}

typedef struct {
    char pad0[0xC50];
    unsigned short xC50;
} Work0x8003ea64;
#define WORK0x8003ea64 (*SCRATCH(Work0x8003ea64 *, 0))
extern void (*D_80089AE0[])(void *);

void func_8003EA64(void *arg0)
{
    D_80089AE0[WORK0x8003ea64->xC50 >> 8](arg0);
}

typedef struct {
    char pad0[0x70];
} Shot0x8003eaac;
typedef struct {
    char pad0[0x4DC];
    Shot0x8003eaac *x4DC;
} Work0x8003eaac;
#define WORK0x8003eaac (*SCRATCH(Work0x8003eaac *, 0))
extern void func_80047958(void *);

void func_8003EAAC(void)
{
    Shot0x8003eaac *p;

    for (p = WORK0x8003eaac->x4DC; p < WORK0x8003eaac->x4DC + 12; p++) {
        func_80047958(p);
    }
}

void func_8003EB18(void)
{
#include "../shared/func_8001B530.inc.c"
}

typedef struct Shot {
    char pad0[0x20];
    MATRIX m;
    short x40;
    short x42;
    short x44;
    signed char x46;
    signed char x47;
    short x48;
    short x4A;
    char pad4c[2];
    signed char x4E;
    signed char x4F;
    int x50;
    char pad54[4];
    short x58;
    short x5A;
    short x5C;
    char pad5e[2];
    SVECTOR x60;
    char pad68[8];
} Shot;
extern S800B27E0 D_800B27E0;
extern Pair D_800B2890[];
void func_80018020(void *, int);
unsigned int func_8003252C(void *);
void func_800339AC(void *, Seg *, SVECTOR *);
int func_80033D4C(void *, void *, SVECTOR *, int);
int func_8003DD34(int, int, unsigned short);
int func_8003F154(void *, void *, int);
void func_80044834(void *, int);
char *func_800474B4(int a, int b);
void func_80047958(void *);
void func_80048048(void *, void *, int, int, int, int);
void func_8004A240(void *, void *, SVECTOR *);
int func_8004A32C(void);
void func_800649A8(int, int, int);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007AA34(SVECTOR *, MATRIX *);

void func_8003EB20(void *arg)
{
    MATRIX m;
    SVECTOR v;
    Seg seg;
    Shot *p;
    unsigned int i;
    int mode;
    int s;
    unsigned int r;
    unsigned char *t;
    AreaTbl *a;
    Unit *u;

    p = WORK->x4DC;
    func_80018020(&D_800B27E0, 0x2B);
    for (i = 0; i < 12; i++, p++) {
        if (!(p->x50 & 1)) continue;
        if (p->x4E != 3) continue;
        if (p->x4F == 3) {
            if (p->x46 != 0) p->x46--; else p->x50 |= 2;
            if (p->x47 != 0) p->x47--; else p->x4F = 0x10;
        }
        mode = p->x4F;
        if (mode < 5) continue;
        if (mode == 0x10) {
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0x400;
            func_8007AA34(&v, &m);
            func_8003F154(&m, &p->x40, 0x13);
            func_80044834(arg, 0x23);
            func_80048048(&p->x40, D_8017DAE0, 0, 0, 0x20, 0);
            func_80047958(p);
            continue;
        }
        *(int *)&p->x58 = *(int *)&p->x40;
        p->x5C = p->x44;
        if (mode == 6 || mode == 10) {
            if (p->x46 == 0) {
                func_80079704(&p->m, &p->x60, &v);
                p->x40 += v.vx;
                p->x42 += v.vy;
                p->x44 += v.vz;
                v.vx = p->x40;
                v.vz = p->x44;
                goto store;
            }
            p->x46--;
            if (D_800B2890[i].a > 0) {
                u = &WORK->units[D_800B2890[i].a];
                if (u->flags & 8) {
                    func_8004A240(func_800474B4(u->id, D_800B2890[i].b) + 0x40, &p->x40, &v);
                    s = (func_8004A32C() & 0x1F) + 0x50;
                    p->x48 += func_8003DD34(p->x48, s, v.vx + 0x400);
                    p->x4A += func_8003DD34(p->x4A, s, v.vy);
                }
            }
        }
        func_80079704(&p->m, &p->x60, &v);
        p->x40 += v.vx;
        p->x42 += v.vy;
        p->x44 += v.vz;
        if (p->x40 > 30000 || p->x40 < -30000 || p->x42 > 30000 || p->x42 < -30000 ||
            p->x44 > 30000 || p->x44 < -30000) {
            func_80047958(p);
            continue;
        }
        D_800B27E0.x50 = 1;
        D_800B27E0.x5A = p->x42 / -1000;
        D_800B27E0.x85 |= 8;
        *(int *)&D_800B27E0.x40 = *(int *)&p->x40;
        D_800B27E0.x44 = p->x44;
        *(int *)&D_800B27E0.x74 = *(int *)&p->x58;
        D_800B27E0.x78 = p->x5C;
        if (func_8003F154(&p->m, &p->x58, p->x4F + 0x10) != 0) goto hit;
        if (func_80033D4C(&p->x40, &p->x58, &v, 8) != 0) {
            r = func_8003252C(&D_800B27E0);
            if (r == 0) continue;
            i = (r >> 16) & 0xFF;
            a = WORK->areas;
            t = (unsigned char *)a + a->offs[r >> 24];
            t += t[1] * 4 + 8;
            seg.x0 = p->x58;
            seg.z0 = p->x5C;
            seg.x1 = p->x40;
            seg.z1 = p->x44;
            func_800339AC(t + i * 4, &seg, &v);
        }
    store:
        p->x40 = v.vx;
        p->x44 = v.vz;
        if (p->x4F == 6) {
            func_80048048(&p->x40, D_8017A31C, 0, 0, 0x20, 0);
            func_80044834(arg, 0x15);
            func_80047958(p);
            return;
        }
    hit:
        switch (p->x4F) {
        case 5:
            func_80048048(&p->x40, D_80179E30, 0, 0, 0x20, 0);
            func_80044834(arg, 0x16);
            break;
        case 6:
            func_80048048(&p->x40, D_8017A040, 0, 0, 0x20, 0);
            func_80044834(arg, 0x15);
            break;
        case 7:
            func_80048048(&p->x40, D_80178CF4, 0, 0, 0x20, 0);
            func_80044834(arg, 0x16);
            func_800649A8(p->x40, p->x42, p->x44);
            break;
        case 10:
            func_80048048(&p->x40, D_80167290, 0, 0, 0x20, 0);
            func_80044834(arg, 0x1D);
            break;
        }
        func_80047958(p);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003F154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003F8CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003FDA0);

typedef struct Obj0x8003fe00 {
    unsigned char pad0[0x40];
    short x40[4];
    short x48;
    short x4A;
    unsigned char pad4C[0x50 - 0x4C];
    int x50;
    unsigned char pad54[0x184 - 0x54];
    unsigned short x184;
    unsigned char pad186[0x260 - 0x186];
} Obj0x8003fe00;
typedef struct Ent0x8003fe00 {
    int key;
    short idx;
    short flag;
} Ent0x8003fe00;
typedef struct Glob0x8003fe00 {
    int x0;
    int x4;
    unsigned char x8[0xC];
    Obj0x8003fe00 *x14;
} Glob0x8003fe00;
#define SPAD0x8003fe00 ((Ent0x8003fe00 *)SPAD_ADDR_2C)
struct Glob0x8004084c;
extern struct Glob0x8004084c *D_800B25DC;
extern int D_800AF11C;
extern int func_8004A0F8(void *a, void *b);
extern int func_8004A2C4(void *p, void *arg);
extern void func_800825E4();
extern int func_8003FDA0();

void func_8003FE00(Obj0x8003fe00 *self, int range)
{
    unsigned int n = 0;
    unsigned int i;
    Obj0x8003fe00 *p;
    Ent0x8003fe00 *e;

    p = ((Obj0x8003fe00 *)WORK->units) + 1;
    for (i = 1; i < D_800AF11C; i++, p++) {
        int f = p->x50;
        if (!(f & 8)) continue;
        if (f & 0x10000) continue;
        {
            int d = func_8004A2C4(self->x40, p->x40);
            if (((self->x4A - d + range) & 0xFFF) <= range * 2) {
                d = func_8004A0F8(self->x40, p->x40);
                e = &SPAD0x8003fe00[n];
                e->key = d;
                e->idx = i;
                n++;
                e->flag = p->x184 & 1;
            }
        }
    }
    func_800825E4(SPAD0x8003fe00, n, 8, func_8003FDA0);
    for (i = 0; i < n; i++) {
        ((Glob0x8003fe00 *)D_800B25DC)->x8[i] = SPAD0x8003fe00[i].idx;
    }
    ((Glob0x8003fe00 *)D_800B25DC)->x0 = n ? SPAD0x8003fe00[0].key : -1;
    ((Glob0x8003fe00 *)D_800B25DC)->x4 = n;
    if (n) {
        ((Glob0x8003fe00 *)D_800B25DC)->x14 = ((Obj0x8003fe00 *)WORK->units) + (unsigned short)SPAD0x8003fe00[0].idx;
    } else {
        ((Glob0x8003fe00 *)D_800B25DC)->x14 = 0;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003FFE4);

typedef struct Elem0x800400f0 {
    unsigned int k;
    int a;
    int b;
    int c;
} Elem0x800400f0;
#define SPAD_2C (*SCRATCH(Elem0x800400f0, 0x2C))
#define SPAD_3C (*SCRATCH(Elem0x800400f0, 0x3C))

void func_800400F0(Elem0x800400f0 *arr, short lo, short hi)
{
    int i = lo;
    int j = hi;

    SPAD_3C = arr[(hi + lo) >> 1];
    do {
        while (arr[i].k < SPAD_3C.k) i++;
        while (SPAD_3C.k < arr[j].k) j--;
        if (j < i) break;
        SPAD_2C = arr[i];
        arr[i] = arr[j];
        arr[j] = SPAD_2C;
        i++; j--;
    } while (i <= j);
    if (lo < j) func_800400F0(arr, lo, j);
    if (i < hi) func_800400F0(arr, i, hi);
}

typedef struct Obj0x800402ec {
    char pad0[0x46];
    unsigned char x46;
    char pad47[0x50 - 0x47];
    int x50;
    char pad54[0x70 - 0x54];
} Obj0x800402ec;
extern void func_80018020(void *, int);

Obj0x800402ec *func_800402EC(void)
{
    Obj0x800402ec *p;
    unsigned char i = 0;

    p = (Obj0x800402ec *)WORK->x4DC;
    do {
        if (!(p->x50 & 1)) {
            func_80018020(p, 0x1C);
            p->x50 = 1;
            p->x46 = i;
            return p;
        }
        i++;
    } while (++p < (Obj0x800402ec *)WORK->x4DC + 12);
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040380);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040444);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800404D4);

typedef struct Obj0x8004084c {
    unsigned char pad0[0x40];
    int x40;
    short x44;
    short pad46;
    short x48;
    short x4A;
    short x4C;
    short pad4E;
    unsigned char pad50[0x14];
    int x64;
    unsigned char pad68[0x134 - 0x68];
    unsigned char *x134;
} Obj0x8004084c;
typedef struct Glob0x8004084c {
    int x0;
    unsigned char pad4[0x10];
    Obj0x8004084c *x14;
    unsigned char pad18[0x58 - 0x18];
    short x58[4];
    short x60[4];
    int x68;
    int x6C;
    Obj0x8004084c *x70;
    Obj0x8004084c *x74;
} Glob0x8004084c;
extern Glob0x8004084c *D_800B25DC;
char *func_800474B4(int a, int b);
extern void func_8004A240();
extern void func_8003FE00();
int func_8003DD34(int, int, unsigned short);

void func_8004084C(Obj0x8004084c *obj, unsigned short mode, short ang)
{
    Obj0x8004084c *base;
    Obj0x8004084c *cur;
    Obj0x8004084c *tgt;
    short *pos;
    int *dst;
    SVECTOR v;

    base = (Obj0x8004084c *)func_800474B4(obj->x64, 0);
    cur = (Obj0x8004084c *)func_800474B4(obj->x64, mode);
    if (mode == 1) {
        obj->x4A = (ang + 0x140) & 0xFFF;
        ang = ang + 0x60;
        pos = D_800B25DC->x60;
        dst = &D_800B25DC->x6C;
        D_800B25DC->x74 = 0;
    } else {
        obj->x4A = (ang - 0x140) & 0xFFF;
        ang = ang - 0x60;
        pos = D_800B25DC->x58;
        dst = &D_800B25DC->x68;
        D_800B25DC->x70 = 0;
    }
    obj->x40 = cur->x40;
    obj->x44 = cur->x44;
    func_8003FE00(obj, 0x180);
    tgt = D_800B25DC->x14;
    if (tgt != 0) {
        if (mode == 1) {
            D_800B25DC->x74 = tgt;
        } else {
            D_800B25DC->x70 = tgt;
        }
        func_8004A240(func_800474B4(tgt->x64, tgt->x134[0xA]) + 0x40, &cur->x40, &v);
        if (v.vx > 0x200) {
            v.vx = 0x200;
        }
        if (v.vx < -0x200) {
            v.vx = -0x200;
        }
        if (mode == 1) {
            pos[0] += func_8003DD34(pos[0], 0x50, v.vx + 0x33E);
        } else {
            pos[0] += func_8003DD34(pos[0], 0x50, v.vx + 0x37A);
        }
        pos[1] += func_8003DD34(pos[1], 0x50, v.vy - ang);
    } else {
        if (mode == 1) {
            pos[0] += func_8003DD34(pos[0], 0x60, 0x33E);
            pos[1] += func_8003DD34(pos[1], 0x60, -10);
        } else {
            pos[0] += func_8003DD34(pos[0], 0x60, 0x37A);
            pos[1] += func_8003DD34(pos[1], 0x60, 10);
        }
    }
    *dst = D_800B25DC->x0;
    cur->x48 = pos[0];
    cur->x4A = pos[1] - base->x4A;
    cur->x4C = 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040AB8);

typedef struct Sub0x80040b94 {
    unsigned char f;
    unsigned char g;
    char pad2[2];
} Sub0x80040b94;
typedef struct Area0x80040b94 {
    unsigned char flags;
    unsigned char n;
    char pad2;
    unsigned char b3;
    unsigned short mask;
    char pad6[2];
    Sub0x80040b94 sub[1];
} Area0x80040b94;
int func_80040DB8(Seg *, int *);

int func_80040B94(SVECTOR *a, SVECTOR *b)
{
    Seg seg;
    int pts[2];
    unsigned int i;
    unsigned int j;
    Area0x80040b94 *e;
    Sub0x80040b94 *s;
    int *p;

    if (WORK->areas->count == 0) return 0;
    seg.x0 = a->vx;
    seg.z0 = a->vz;
    seg.x1 = b->vx;
    seg.z1 = b->vz;
    for (i = 0; i < WORK->areas->count; i++) {
        e = (Area0x80040b94 *)((char *)WORK->areas + WORK->areas->offs[i]);
        if (!(e->flags & 1)) continue;
        if (!((e->mask >> (a->vy / -1000)) & 1) && !((e->mask >> (b->vy / -1000)) & 1)) continue;
        if (e->b3 & 4) continue;
        s = e->sub;
        p = (int *)&e->sub[e->n];
        for (j = 0; j < e->n; j++, s++) {
            pts[0] = *p++;
            pts[1] = *p;
            if ((s->f & 1) && !(s->g & 4)) {
                if (func_80040DB8(&seg, pts)) return 1;
            }
        }
    }
    return 0;
}

int func_80040DB8(Seg *a, int *p)
{
    Seg *b = (Seg *)p;
    int ax0, ax1, bx0, bx1;
    int az0, az1, bz0, bz1;
    int dax, dbz, daz, dbx;
    int d, s, t, ox, oz;

    ax0 = a->x0;
    ax1 = a->x1;
    bx0 = b->x0;
    bx1 = b->x1;
    if (ax1 < ax0) {
        if (bx0 < bx1) {
            if (ax0 < bx0) return 0;
            if (bx1 < ax1) return 0;
        } else {
            if (ax0 < bx1) return 0;
            if (bx0 < ax1) return 0;
        }
    } else {
        if (bx0 < bx1) {
            if (ax1 < bx0) return 0;
            if (bx1 < ax0) return 0;
        } else {
            if (ax1 < bx1) return 0;
            if (bx0 < ax0) return 0;
        }
    }
    az0 = a->z0;
    az1 = a->z1;
    bz0 = b->z0;
    bz1 = b->z1;
    if (az1 < az0) {
        if (bz0 < bz1) {
            if (az0 < bz0) return 0;
            if (bz1 < az1) return 0;
        } else {
            if (az0 < bz1) return 0;
            if (bz0 < az1) return 0;
        }
    } else {
        if (bz0 < bz1) {
            if (az1 < bz0) return 0;
            if (bz1 < az0) return 0;
        } else {
            if (az1 < bz1) return 0;
            if (bz0 < az0) return 0;
        }
    }
    dax = ax1 - ax0;
    dbz = bz1 - bz0;
    daz = az1 - az0;
    dbx = bx1 - bx0;
    d = dax * dbz - daz * dbx;
    if (d == 0) return 0;
    oz = az0 - bz0;
    ox = ax0 - bx0;
    s = oz * dbx - ox * dbz;
    t = oz * dax - ox * daz;
    if (!((d > 0 && s >= 0 && s <= d) || (d < 0 && s >= d && s <= 0))) return 0;
    if (!((d > 0 && t >= 0 && t <= d) || (d < 0 && t >= d && t <= 0))) return 0;
    return 1;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040F7C);

extern int D_800181D4;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);


extern unsigned short *D_80089CE4[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);




extern void func_80045C18(int arg0);


extern void func_80043E1C(int);


extern void func_80045A20(int, int);


extern signed char D_800B32FB;
extern void func_80044A88(int, int, int, int);



extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


