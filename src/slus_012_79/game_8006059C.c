#include "common.h"
#include "dc2.h"
#include "../shared/func_8005A34C.h"
#include "../shared/func_800610A0.h"
#include "../shared/func_800631D8.h"
#include "../shared/func_8006600C.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;















extern void func_80034C48(int, int, int);


extern int func_80034CD8(int, int);




INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_8006059C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060738);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060848);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060968);

void func_80060A24(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060A2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060E18);

int func_80060EAC(void)
{
#include "../shared/func_800469D0.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060EB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80060EF0);

typedef struct {
    int pc;
    char pad04[0x80];
    int *sp;
} Ctx0x80060f2c;
typedef struct {
    char pad0[0x7C];
    int x7C;
    char pad80[0x260 - 0x80];
} Unit0x80060f2c;
extern void func_800484F0(Unit *, int, int, int, int);

int func_80060F2C(Ctx0x80060f2c *ctx)
{
    int *sp = ctx->sp;
    Unit *u = &WORK->units[sp[4]];
    if (sp[0] == 0) {
        ((Unit0x80060f2c *)u)->x7C = 0;
    }
    func_800484F0(u, ctx->sp[3], ctx->sp[1], ctx->sp[2], ctx->sp[0]);
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    unsigned char *src;
} Obj0x80060fbc;
extern unsigned char D_800B49D7;

int func_80060FBC(Obj0x80060fbc *arg0)
{
    D_800B49D7 = *arg0->src;
    arg0->pc += 2;
    return 0;
}

typedef struct {
    int pc;
    char pad04[0x80];
    int *sp;
} Ctx0x80060fe8;
extern void func_80041DC0(int, int);

int func_80060FE8(Ctx0x80060fe8 *ctx)
{
    int *sp = ctx->sp;
    int v = sp[1];

    if (v < 0) {
        func_80041DC0(((unsigned char *)v)[1], ((unsigned char *)v)[4]);
    } else {
        func_80041DC0(v, sp[0]);
    }
    ctx->pc += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80061054);

#define SHARED_F0 func_80034D88
int func_800610A0(ScriptCtx_s800610A0 *ctx)
{
#include "../shared/func_800610A0.inc.c"
}

typedef struct {
    int pc;
    char pad04[0x80];
    int *arg;
} Ctx0x800610ec;
extern void func_80021EF0(int a, int b);

int func_800610EC(Ctx0x800610ec *ctx)
{
    func_80021EF0(ctx->arg[0] * 30, 0);
    ctx->pc += 2;
    return 0;
}

#define SHARED_F0 func_8002EB14
int func_8006113C(ScriptCtx_s800610A0 *ctx)
{
#include "../shared/func_800610A0.inc.c"
}

typedef struct {
    int pc;
    char pad04[0x80];
    int *arg;
} Ctx0x80061188;
extern int func_80034CD8(int, int);

int func_80061188(Ctx0x80061188 *ctx)
{
    int ret;

    if (func_80034CD8(ctx->arg[2], ctx->arg[1]) != ctx->arg[0]) {
        ret = 1;
    } else {
        ctx->pc += 2;
        ret = 0;
    }
    return ret;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_800611EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_800614B4);

extern void func_8004A4D4(Unit *, int, int, int);

int func_800614F0(Ctx8005EEF4 *ctx)
{
    int *sp = ctx->sp;
    func_8004A4D4(&WORK->units[sp[3]], sp[2], *(short *)&sp[1], *(short *)&sp[0]);
    ctx->pc += 2;
    return 0;
}

typedef struct {
    unsigned short f0;
    char pad2[2];
    unsigned short f4;
    char pad6[2];
    unsigned short f8;
    char padA[2];
    unsigned short fC;
    char padE[2];
    union {
        int w;
        unsigned short h;
    } f10;
} Src0x8006155c;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x8006155c *src;
} Obj0x8006155c;
extern int func_80034CD8(int, int);
extern void func_80051F8C(int, int, int, int, int);

int func_8006155C(Obj0x8006155c *o)
{
    Src0x8006155c *s;
    unsigned short v;

    if (func_80034CD8(2, 6) != 0) {
        return 1;
    }
    if (o->src->f10.w == 1) {
        v = (o->src->f0 << 8) | 0xFF;
    } else {
        v = o->src->f0;
    }
    s = o->src;
    func_80051F8C(s->f10.h, s->fC, s->f8, s->f4, v);
    o->pc += 2;
    return 1;
}

typedef struct {
    int w0;
} Src0x800615ec;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x800615ec *src;
} Obj0x800615ec;
extern int func_80035A88(int a, int b);

int func_800615EC(Obj0x800615ec *o)
{
    int v = o->src->w0;
    int i;

    if (v == 0xFFFF) {
        for (i = 0; i < 0x5C; i++) {
            WORK->xE6C[i] = 0;
        }
    } else {
        WORK->xE6C[func_80035A88((v >> 8) & 0xFF, *(unsigned char *)o->src)] = 0;
    }
    o->pc += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80061680);

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
    int x3C;
    int x40;
    int x44;
    int x48;
} Args0x80061798;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80061798 *x84;
} Ctx0x80061798;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    short h14;
    short h16;
    short h18;
    short h1A;
    short h1C;
    short h1E;
    unsigned char b20;
    unsigned char b21;
    char pad22[0x28 - 0x22];
} Ent0x80061798;

int func_80061798(Ctx0x80061798 *ctx)
{
    Ent0x80061798 *e = (Ent0x80061798 *)WORK->x5FC + ctx->x84->x48;

    e->b1 = 0;
    e->b0 = ctx->x84->x44 | 9;
    e->b2 = ctx->x84->x40;
    e->h4 = ctx->x84->x3C;
    e->h6 = ctx->x84->x38;
    e->h8 = ctx->x84->x34;
    e->hA = ctx->x84->x30;
    e->hC = ctx->x84->x2C;
    e->hE = ctx->x84->x28;
    e->h10 = ctx->x84->x24;
    e->h12 = ctx->x84->x20;
    e->h14 = ctx->x84->x1C;
    e->h16 = ctx->x84->x18;
    e->h18 = ctx->x84->x14;
    e->h1A = ctx->x84->x10;
    e->h1C = ctx->x84->xC;
    e->h1E = ctx->x84->x8;
    e->b20 = ctx->x84->x4;
    e->b21 = ctx->x84->x0;
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
} Args0x80061948;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80061948 *x84;
} Ctx0x80061948;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    unsigned char b14;
    char pad15[0x18 - 0x15];
    int w18;
    char pad1C[0x28 - 0x1C];
} Ent0x80061948;

int func_80061948(Ctx0x80061948 *ctx)
{
    Ent0x80061948 *e = (Ent0x80061948 *)WORK->x5FC + ctx->x84->x30;

    e->b1 = 1;
    e->b0 = ctx->x84->x2C | 1;
    e->b2 = ctx->x84->x28;
    e->h4 = ctx->x84->x24;
    e->h6 = ctx->x84->x20;
    e->h8 = ctx->x84->x1C;
    e->hA = ctx->x84->x18;
    e->hC = ctx->x84->x14;
    e->hE = ctx->x84->x10;
    e->h10 = ctx->x84->xC;
    e->h12 = ctx->x84->x8;
    e->b14 = ctx->x84->x4;
    e->w18 = ctx->x84->x0;
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
} Args0x80061a84;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80061a84 *x84;
} Ctx0x80061a84;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    int w14;
    char pad18[0x28 - 0x18];
} Ent0x80061a84;

int func_80061A84(Ctx0x80061a84 *ctx)
{
    Ent0x80061a84 *e = (Ent0x80061a84 *)WORK->x5FC + ctx->x84->x30;

    e->b1 = 2;
    e->b0 = ctx->x84->x2C | 1;
    e->b2 = ctx->x84->x28;
    e->h4 = ctx->x84->x24;
    e->h6 = ctx->x84->x20;
    e->h8 = ctx->x84->x1C;
    e->hA = ctx->x84->x18;
    e->hC = ctx->x84->x14;
    e->hE = ctx->x84->x10;
    e->h10 = ctx->x84->xC;
    e->h12 = ctx->x84->x8;
    e->w14 = ctx->x84->x4;
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
} Args0x80061bac;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80061bac *x84;
} Ctx0x80061bac;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    unsigned char b14;
    unsigned char b15;
    unsigned char b16;
    char pad17[0x28 - 0x17];
} Ent0x80061bac;

int func_80061BAC(Ctx0x80061bac *ctx)
{
    Ent0x80061bac *e = (Ent0x80061bac *)WORK->x5FC + ctx->x84->x34;

    e->b1 = 3;
    e->b0 = ctx->x84->x30 | 1;
    e->b2 = ctx->x84->x2C;
    e->h4 = ctx->x84->x28;
    e->h6 = ctx->x84->x24;
    e->h8 = ctx->x84->x20;
    e->hA = ctx->x84->x1C;
    e->hC = ctx->x84->x18;
    e->hE = ctx->x84->x14;
    e->h10 = ctx->x84->x10;
    e->h12 = ctx->x84->xC;
    e->b14 = ctx->x84->x8;
    e->b15 = ctx->x84->x4;
    e->b16 = ctx->x84->x0;
    ctx->pc += 2;
    return 0;
}

int func_80061CFC(int *a0)
{
#include "../shared/func_8005ECB4.inc.c"
}

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
    int x3C;
    int x40;
} Args0x80061d14;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80061d14 *x84;
} Ctx0x80061d14;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    int w14;
    short h18;
    unsigned char b1A;
    unsigned char b1B;
    char pad1C[0x28 - 0x1C];
} Ent0x80061d14;
typedef struct {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    unsigned char x46;
    char pad47[0x4E - 0x47];
    unsigned char x4E;
    char pad4F;
    int x50;
    char pad54[0x68 - 0x54];
    int x68;
    short x6C;
    short x6E;
} Shot0x80061d14;
extern int func_80034CD8(int, int);

int func_80061D14(Ctx0x80061d14 *ctx)
{
    Args0x80061d14 *a = ctx->x84;
    Ent0x80061d14 *e = (Ent0x80061d14 *)WORK->x5FC + a->x40;
    Shot0x80061d14 *s;

    if (func_80034CD8(5, a->x14) == 0 || ctx->x84->x14 == 0) {
        e->b1 = 5;
        e->b0 = ctx->x84->x3C | 1;
        e->b2 = ctx->x84->x38;
        e->h4 = ctx->x84->x34;
        e->h6 = ctx->x84->x30;
        e->h8 = ctx->x84->x2C;
        e->hA = ctx->x84->x28;
        e->hC = ctx->x84->x24;
        e->hE = ctx->x84->x20;
        e->h10 = ctx->x84->x1C;
        e->h12 = ctx->x84->x18;
        e->h18 = ctx->x84->x14;
        e->w14 = ctx->x84->x10;
        e->b1A = ctx->x84->xC;
        e->b1B = ctx->x84->x4 + 12;
        if (ctx->x84->x4 != 0xFF) {
            s = (Shot0x80061d14 *)WORK->x4DC + (ctx->x84->x4 + 12);
            s->x50 = 7;
            s->x4E = 1;
            s->x46 = ctx->x84->x8;
            s->x6C = 0;
            s->x6E = 10;
            s->x68 = ctx->x84->x0;
            s->x40 = (ctx->x84->x34 + ctx->x84->x2C + ctx->x84->x24 + ctx->x84->x1C) / 4;
            s->x42 = -(ctx->x84->x38 * 1000) - 100;
            s->x44 = (ctx->x84->x30 + ctx->x84->x28 + ctx->x84->x20 + ctx->x84->x18) / 4;
        }
    }
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
} Args0x80061fd4;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80061fd4 *x84;
} Ctx0x80061fd4;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    int w14;
    char pad18[0x28 - 0x18];
} Ent0x80061fd4;

int func_80061FD4(Ctx0x80061fd4 *ctx)
{
    Ent0x80061fd4 *e = (Ent0x80061fd4 *)WORK->x5FC + ctx->x84->x2C;

    e->b1 = 6;
    e->b0 = ctx->x84->x28 | 1;
    e->b2 = ctx->x84->x24;
    e->h4 = ctx->x84->x20;
    e->h6 = ctx->x84->x1C;
    e->h8 = ctx->x84->x18;
    e->hA = ctx->x84->x14;
    e->hC = ctx->x84->x10;
    e->hE = ctx->x84->xC;
    e->h10 = ctx->x84->x8;
    e->h12 = ctx->x84->x4;
    e->w14 = ctx->x84->x0 + 0xC;
    ctx->pc += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_800620FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80062210);

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
} Args0x80062324;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80062324 *x84;
} Ctx0x80062324;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    int w14;
    unsigned char b18;
    unsigned char b19;
    char pad1A[0x28 - 0x1A];
} Ent0x80062324;

int func_80062324(Ctx0x80062324 *ctx)
{
    Ent0x80062324 *e = (Ent0x80062324 *)WORK->x5FC + ctx->x84->x34;

    e->b1 = 9;
    e->b0 = ctx->x84->x30 | 1;
    e->b2 = ctx->x84->x2C;
    e->h4 = ctx->x84->x28;
    e->h6 = ctx->x84->x24;
    e->h8 = ctx->x84->x20;
    e->hA = ctx->x84->x1C;
    e->hC = ctx->x84->x18;
    e->hE = ctx->x84->x14;
    e->h10 = ctx->x84->x10;
    e->h12 = ctx->x84->xC;
    e->b19 = ctx->x84->x8;
    e->b18 = ctx->x84->x4;
    e->w14 = ctx->x84->x0;
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
} Args0x80062474;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Args0x80062474 *x84;
} Ctx0x80062474;
typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    char pad3;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    unsigned char b14;
    unsigned char b15;
    unsigned char b16;
    unsigned char b17;
    int w18;
    char pad1C[0x28 - 0x1C];
} Ent0x80062474;
typedef struct {
    char pad0[0x64];
    unsigned char b64;
    unsigned char b65;
    unsigned char b66;
    char pad67[0xAC - 0x67];
} Obj0x80062474;
extern int func_80034CD8(int, int);

int func_80062474(Ctx0x80062474 *ctx)
{
    Ent0x80062474 *e;
    Obj0x80062474 *o;

    if (func_80034CD8(8, ctx->x84->x8) == 0) {
        e = (Ent0x80062474 *)WORK->x5FC + ctx->x84->x38;
        e->b1 = 10;
        e->b0 = ctx->x84->x34 | 1;
        e->b2 = ctx->x84->x30;
        e->h4 = ctx->x84->x2C;
        e->h6 = ctx->x84->x28;
        e->h8 = ctx->x84->x24;
        e->hA = ctx->x84->x20;
        e->hC = ctx->x84->x1C;
        e->hE = ctx->x84->x18;
        e->h10 = ctx->x84->x14;
        e->h12 = ctx->x84->x10;
        e->b14 = ctx->x84->xC;
        e->b15 = ctx->x84->x8;
        e->w18 = ctx->x84->x4;
        e->b17 = ctx->x84->x0;
        e->b16 = 3;
    } else if (ctx->x84->xC == 1) {
        Args0x80062474 *a = ctx->x84;
        o = (Obj0x80062474 *)WORK->x4E4 + a->x4;
        o->b64 = 0;
        o->b65 = 0xFF;
        o->b66 = 0;
    }
    ctx->pc += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_8006266C);

extern void (*D_8008A3B0[])(Elem28 *);

int func_80062794(Ctx *ctx)
{
    Elem28 *e;
    int r;

    e = WORK->x5FC + *(int *)ctx->arg;
    D_8008A3B0[e->pad[0]](e);
    r = e->pad[0] == 5;
    ctx->pc += 2;
    return r;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80062820);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_800629BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80062AC0);

typedef struct {
    int pc;
    char pad04[0x80];
    int *sp;
    char pad88[0x9F - 0x88];
    signed char unit;
} Ctx0x80062e80;
extern void func_800484F0(Unit *, int, int, int, int);

int func_80062E80(Ctx0x80062e80 *ctx)
{
    int *sp = ctx->sp;
    func_800484F0(&WORK->units[ctx->unit], sp[3], sp[1], sp[2], sp[0]);
    ctx->pc += 2;
    return 0;
}

typedef struct {
    char pad0[0x54];
    char *x54;
    char pad58[0x70 - 0x58];
} Shot0x80062ef4;
typedef struct {
    char pad0[0x60];
    char *x60;
    char pad64[0xAC - 0x64];
} Ent0x80062ef4;
typedef struct {
    int x0;
    char pad4[0x84 - 0x4];
    int *x84;
    char pad88[0x9E - 0x88];
    unsigned char x9E;
    signed char x9F;
} Obj0x80062ef4;
extern char *func_800474B4(int a, int b);

int func_80062EF4(Obj0x80062ef4 *obj)
{
    char **dst;
    int *p;

    dst = 0;
    switch (obj->x9E) {
    case 1:
        dst = &((Shot0x80062ef4 *)WORK->x4DC)[obj->x9F + 12].x54;
        break;
    case 2:
        dst = &((Ent0x80062ef4 *)WORK->x4E4)[obj->x9F].x60;
        break;
    }
    p = obj->x84;
    switch (p[2]) {
    case 0:
        *dst = func_800474B4(WORK->units[p[1]].id, p[0]);
        break;
    case 1:
        *dst = func_800474B4((int)&((Shot0x80062ef4 *)WORK->x4DC)[p[1] + 12], p[0]);
        break;
    }
    obj->x0 += 2;
    return 0;
}

typedef struct {
    int pc;
    char pad04[0x84];
    int val;
} ScriptCtx0x80063040;
extern int func_80081CB4(void);
extern void func_8005EC80(ScriptCtx0x80063040 *ctx, int v);

int func_80063040(ScriptCtx0x80063040 *ctx)
{
    int v = func_80081CB4() & 0xFFFF;
    ctx->val = v;
    func_8005EC80(ctx, v);
    ctx->pc += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80063084);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_80063158);

typedef struct {
    int pc;
    char pad04[0x80];
    int *arg;
} ScriptCtx0x80063194;
extern int func_8001C680(int a, int b);

int func_80063194(ScriptCtx0x80063194 *ctx)
{
    func_8001C680(0, ctx->arg[0]);
    ctx->pc += 2;
    return 0;
}

#define SHARED_F0 func_80041994
int func_800631D8(ScriptCtx_s800631D8 *ctx)
{
#include "../shared/func_800631D8.inc.c"
}

extern int func_8001D9D4(void);

int func_8006321C(int *pc)
{
    int ret;

    if (func_8001D9D4() != 0) {
        ret = 1;
    } else {
        *pc += 2;
        ret = 0;
    }
    return ret;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8006059C", func_8006325C);

extern void func_80037068(int);
extern void func_80034C48(int, int, int);
extern void func_8002EB14(int, int, int);
extern int D_800B4A60;
extern int D_800B4A64;

int func_8006335C(Obj0x8006335c *o)
{
    func_80037068(*o->src);
    D_800B4A60 = WORK->x6B8;
    WORK->x6B8 = 0;
    D_800B4A64 = WORK->x6B4;
    func_80034C48(1, 0xC, 1);
    func_8002EB14(0, 3, 1);
    o->pc += 2;
    return 1;
}

typedef struct {
    int x0;
} Obj0x800633e8;
extern void func_800371A0(void);
extern int func_80034CD8(int, int);
extern void func_8002EB14(int, int, int);
extern int D_800B4A60;
extern int D_800B4A64;

int func_800633E8(Obj0x800633e8 *p)
{
    func_800371A0();
    if (func_80034CD8(2, 0x17) == 0) {
        func_8002EB14(1, 1, 1);
        WORK->x6B8 = D_800B4A60;
        WORK->x6B4 = D_800B4A64;
        p->x0 += 2;
    }
    return 1;
}


#define SHARED_F0 func_80030DDC


#define SHARED_F0 func_80044AB4

#define SHARED_F0 func_80044B80

extern int func_80034CD8(int, int);


extern int func_80034CD8(int, int);
extern void func_8001D9E4(void);
extern void func_8001E4A0(int, int);


extern unsigned short *func_800458F0(int a, int b);


extern Elem28 D_800B4A68[];


extern void func_80034C48(int, int, int);
extern unsigned char D_800B49D4;


extern int func_80034CD8(int, int);
extern void func_80065D0C(int, int, char *, int);
extern char D_8008EB88[];
extern char D_8008E98E[];


extern int D_800B4F74;



extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;




#define SHARED_D0 D_8008F780




#define SHARED_D0 D_8008F78C

#define SHARED_F0 func_800661FC

