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








typedef struct Ctx0x8005eda0 {
    unsigned char *pc;
    char pad04[0x84 - 0x4];
    int v84;
    int arr[1];
} Ctx0x8005eda0;
extern int func_8005EA34();
extern int func_8005EB34();
extern void func_8005EC80();

int func_8005EDA0(Ctx0x8005eda0 *ctx)
{
    int idx;
    int r;
    int a;
    int b;

    a = ctx->pc[3];
    b = *(short *)(ctx->pc + 4);
    idx = ctx->pc[2];
    r = func_8005EA34(ctx, a, b);
    if (idx != 0xFF) {
        a = ctx->arr[idx];
    } else {
        a = ctx->v84;
        r <<= 2;
    }
    a = func_8005EB34(a, r, ctx->pc[1]);
    if (idx != 0xFF) {
        ctx->arr[idx] = a;
    } else {
        ctx->v84 = a;
    }
    func_8005EC80(ctx, a);
    ctx->pc += 6;
    return 0;
}

typedef struct Ctx0x8005ee58 {
    unsigned char *pc;
    char pad04[0x80];
    int *sp;
    char pad88[0x13];
    unsigned char x9B;
    char pad9C;
    unsigned char x9D;
} Ctx0x8005ee58;

int func_8005EE58(Ctx0x8005ee58 *ctx)
{
    if ((signed char)ctx->x9D != 0) {
        ctx->x9D--;
        ctx->pc = (unsigned char *)*ctx->sp++;
        return 0;
    }
    ctx->x9B = 0;
    return 1;
}

typedef struct Ctx0x8005ee98 {
    unsigned char *pc;
    char pad04[0x84 - 0x4];
    int *sp;
} Ctx0x8005ee98;
extern int func_8005EA34();

int func_8005EE98(Ctx0x8005ee98 *ctx)
{
    int v;

    v = func_8005EA34(ctx, ctx->pc[1], *(short *)(ctx->pc + 2));
    *--ctx->sp = v;
    ctx->pc += 4;
    return 0;
}

extern void func_8005EAB4(Ctx8005EEF4 *ctx, int a, int b, int c);

int func_8005EEF4(Ctx8005EEF4 *ctx)
{
    unsigned char *pc;
    int v = *ctx->sp++;
    pc = ctx->pc;
    func_8005EAB4(ctx, pc[1], *(short *)(pc + 2), v);
    pc = ctx->pc;
    pc += 4;
    ctx->pc = pc;
    return 0;
}

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

s32 func_8005EF48(void *arg0) {
    u8 temp_a1;
    void *temp_v1;
    void *var_v0;

    temp_v1 = M2C_FIELD(arg0, void **, 0);
    temp_a1 = M2C_FIELD(temp_v1, u8 *, 1);
    if ((((s32) M2C_FIELD(arg0, u8 *, 0x9C) >> temp_a1) & 1) || (temp_a1 == 0)) {
        var_v0 = temp_v1 + M2C_FIELD(temp_v1, s16 *, 2);
    } else {
        var_v0 = temp_v1 + 4;
    }
    M2C_FIELD(arg0, void **, 0) = var_v0;
    return 0;
}

typedef struct Ctx0x8005ef8c {
    unsigned char *pc;
    char pad04[0x84 - 0x4];
    int *sp;
    char pad88[0x9D - 0x88];
    unsigned char x9D;
} Ctx0x8005ef8c;

int func_8005EF8C(Ctx0x8005ef8c *ctx)
{
    int *p;

    *--ctx->sp = (int)(ctx->pc + 4);
    p = (int *)WORK->x5F4;
    p += *(short *)(ctx->pc + 2);
    ctx->pc = (unsigned char *)WORK->x5F4 + *p;
    ctx->x9D++;
    return 0;
}

int func_8005EFE4(int *a0)
{
#include "../shared/func_8005ECB4.inc.c"
}

typedef struct {
    unsigned short h0;
} Src0x8005effc;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x8005effc *src;
    char pad88[0x98 - 0x88];
    unsigned short x98;
    char pad9A;
    unsigned char x9B;
} Obj0x8005effc;

int func_8005EFFC(Obj0x8005effc *o)
{
    o->x9B = 2;
    o->x98 = o->src->h0;
    o->pc += 2;
    return 1;
}

extern int func_80065D0C(int, int, char *, int);
extern unsigned char D_800B49D5;

int func_8005F024(Ctx8005EEF4 *ctx)
{
    func_80065D0C(0x19, 0xB0, WORK->x5F4 + *ctx->sp, 2);
    D_800B49D5 = 0xFF;
    ctx->pc += 2;
    return 0;
}


int func_8005F088(Ctx *c)
{
    unsigned char *a = c->arg;
    c->b9E = a[4];
    c->b9F = a[0];
    c->pc += 2;
    return 0;
}

typedef struct Shot0x8005f0b4 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    short pad46;
    short x48;
    short x4A;
    short x4C;
    unsigned char x4E;
    char pad4F;
    int x50;
    char pad54[0x68 - 0x54];
    int x68;
    short x6C;
    short x6E;
} Shot0x8005f0b4;
typedef struct Src0x8005f0b4 {
    char pad0[0x10];
    unsigned short x10;
    short pad12;
    unsigned short x14;
    short pad16;
    unsigned short x18;
    short pad1A;
    unsigned short x1C;
    short pad1E;
    unsigned short x20;
    short pad22;
    unsigned short x24;
    short pad26;
    int x28;
    unsigned short x2C;
    short pad2E;
    unsigned short x30;
    short pad32;
    unsigned char x34;
    char pad35[3];
    int x38;
} Src0x8005f0b4;
typedef struct Obj0x8005f0b4 {
    int x0;
    char pad4[0x84 - 4];
    Src0x8005f0b4 *x84;
} Obj0x8005f0b4;
extern void func_80018020(void *, int);

int func_8005F0B4(Obj0x8005f0b4 *obj)
{
    Shot0x8005f0b4 *s;

    s = &((Shot0x8005f0b4 *)WORK->x4DC)[obj->x84->x38 + 12];
    func_80018020(s, 0x1C);
    s->x50 = 7;
    s->x4E = obj->x84->x34;
    s->x6C = obj->x84->x30;
    s->x6E = obj->x84->x2C;
    s->x68 = obj->x84->x28;
    s->x40 = obj->x84->x24;
    s->x42 = obj->x84->x20;
    s->x44 = obj->x84->x1C;
    s->x48 = obj->x84->x18;
    s->x4A = obj->x84->x14;
    s->x4C = obj->x84->x10;
    obj->x0 += 2;
    return 0;
}

typedef struct Ent0x8005f1ec {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x50 - 0x46];
    int x50;
    char pad54[0x58 - 0x54];
    unsigned char x58;
    char pad59[0x64 - 0x59];
    unsigned char x64;
    unsigned char x65;
    unsigned char x66;
    unsigned char x67;
    char pad68[0x6C - 0x68];
    short x6C;
    short x6E;
    char pad70[0x98 - 0x70];
    short x98[6];
    char padA4[0xAC - 0xA4];
} Ent0x8005f1ec;
typedef struct Src0x8005f1ec {
    unsigned short x0;
    short pad2;
    unsigned short x4;
    short pad6;
    unsigned short x8;
    short padA;
    unsigned short xC;
    short padE;
    unsigned short x10;
    short pad12;
    unsigned short x14;
    short pad16;
    unsigned short x18;
    short pad1A;
    unsigned short x1C;
    short pad1E;
    unsigned short x20;
    short pad22;
    unsigned char x24;
    char pad25[3];
    int x28;
} Src0x8005f1ec;
typedef struct Obj0x8005f1ec {
    int x0;
    char pad4[0x84 - 4];
    Src0x8005f1ec *x84;
} Obj0x8005f1ec;

int func_8005F1EC(Obj0x8005f1ec *obj)
{
    Ent0x8005f1ec *s;
    short *p;

    s = &((Ent0x8005f1ec *)WORK->x4E4)[obj->x84->x28];
    func_80018020(s, 0x2B);
    s->x50 = 1;
    s->x6C = 0x1E;
    s->x6E = 0x1E;
    s->x67 = 0x2C;
    s->x64 = 0x80;
    s->x65 = 0x80;
    s->x66 = 0x80;
    s->x58 = obj->x84->x24;
    s->x40 = obj->x84->x20;
    s->x42 = obj->x84->x1C;
    s->x44 = obj->x84->x18;
    s->x98[0] = obj->x84->x14;
    p = s->x98;
    p[1] = obj->x84->x10;
    p[2] = obj->x84->xC;
    p[3] = obj->x84->x8;
    p[4] = obj->x84->x4;
    p[5] = obj->x84->x0;
    obj->x0 += 2;
    return 0;
}

typedef struct Arg0x8005f354 {
    short a;
    char pad02[0x4 - 0x2];
    signed char b;
} Arg0x8005f354;
typedef struct Ctx0x8005f354 {
    unsigned char *pc;
    char pad04[0x84 - 0x4];
    Arg0x8005f354 *arg;
} Ctx0x8005f354;
extern int func_80034CD8(int, int);
extern void func_8005E854(int, int);

int func_8005F354(Ctx0x8005f354 *ctx)
{
    if (func_80034CD8(0, 9) == 0) {
        func_8005E854(ctx->arg->b, ctx->arg->a);
    }
    ctx->pc += 2;
    return 0;
}

typedef struct {
    int pc;
    char pad04[0x80];
    signed char *arg;
    char pad88[0x13];
    unsigned char f9B;
} Ctx0x8005f3b0;
extern void func_8005E8F8(int);

int func_8005F3B0(Ctx0x8005f3b0 *ctx)
{
    int ret;

    func_8005E8F8(ctx->arg[0]);
    if (ctx->f9B != 0) {
        ctx->pc += 2;
        ret = 0;
    } else {
        ret = 1;
    }
    return ret;
}

typedef struct Ctx0x8005f410 {
    unsigned char *pc;
    char pad4[0x84 - 0x4];
    int *arg;
} Ctx0x8005f410;
typedef struct Elem0x8005f410 {
    char pad0[0x9B];
    unsigned char x9B;
    char pad9C[0xA4 - 0x9C];
} Elem0x8005f410;
extern Elem0x8005f410 *D_800B49C8;

int func_8005F410(Ctx0x8005f410 *ctx)
{
    D_800B49C8[ctx->arg[0]].x9B |= 0x80;
    ctx->pc += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F45C);

typedef struct Elem0x8005f4bc {
    char *x0;
    char pad4[0x80 - 0x4];
    int x80;
    int *x84;
    char pad88[0x9B - 0x88];
    unsigned char x9B;
    char pad9C;
    unsigned char x9D;
} Elem0x8005f4bc;

int func_8005F4BC(Elem0x8005f4bc *e)
{
    int *p;
    char *v;

    p = (int *)WORK->x5F4 + *e->x84;
    func_80018020(e, 0x29);
    e->x9B = 1;
    v = WORK->x5F4 + *p;
    e->x84 = &e->x80;
    e->x9D = 0;
    e->x0 = v;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F540);

typedef struct {
    int x0;
    int x4;
    int x8;
} Args0x8005f714;
typedef struct {
    int x0;
    char pad4[0x84 - 0x4];
    Args0x8005f714 *x84;
} Obj0x8005f714;
extern void func_80034C48(int, int, int);

int func_8005F714(Obj0x8005f714 *p)
{
    Args0x8005f714 *a = p->x84;

    if (a->x8 == 1 && a->x4 == 0xFF) {
        if (a->x0 == a->x8) {
            WORK->x6B4 = -0x41;
        } else {
            WORK->x6B4 = 0;
        }
    } else {
        func_80034C48(p->x84->x8, p->x84->x4, p->x84->x0);
    }
    p->x0 += 2;
    return 0;
}

typedef struct {
    int f0;
    int f4;
} Src0x8005f7b8;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x8005f7b8 *src;
    int f88;
    char pad8C[0x9C - 0x8C];
    unsigned char f9C;
} Obj0x8005f7b8;
extern int func_80034CD8(int, int);

int func_8005F7B8(Obj0x8005f7b8 *o)
{
    if (func_80034CD8(o->src->f4, o->src->f0) != 0) {
        o->f9C = 0x40;
        o->f88 = 1;
    } else {
        o->f9C = 2;
        o->f88 = 0;
    }
    o->pc += 2;
    return 0;
}

typedef struct Obj0x8005f824 {
    int f0;
    unsigned char pad4[0x9A];
    unsigned char state;
} Obj0x8005f824;
extern void (*D_8008A368[])(Obj0x8005f824 *);

int func_8005F824(Obj0x8005f824 *obj)
{
    D_8008A368[obj->state](obj);
    obj->f0 += 2;
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F878);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005FB44);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005FCD0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005FEE0);

void func_8005FFB8(void)
{
#include "../shared/func_8001B530.inc.c"
}

extern void func_8001D7D0();
extern int func_80079AA4();

void func_8005FFC0(void)
{
    SVECTOR v;
    Cam0x800266dc *cam = (Cam0x800266dc *)WORK->x318;

    func_8001D7D0(&cam->x28, cam->x30, &cam->x20, cam->x38);
    cam->x20 &= 0xFFF;
    cam->x22 &= 0xFFF;
    cam->x24 &= 0xFFF;
    v.vx = -cam->x20;
    v.vy = -cam->x22;
    v.vz = cam->x24;
    func_80079AA4(&v, cam);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8006004C);

typedef struct Ctx0x80060460 {
    char pad0[0x84];
    unsigned char *arg;
    char pad88[0x9F - 0x88];
    signed char x9F;
} Ctx0x80060460;

void func_80060460(Ctx0x80060460 *ctx)
{
    unsigned char *arg;
    Rec0x80060460 *r;

    r = (Rec0x80060460 *)WORK->x5E4 + ctx->x9F;
    arg = ctx->arg;
    switch (((int *)arg)[1]) {
    case 30:
        r->x4 = arg[0];
        break;
    case 31:
        r->x6 = arg[0];
        break;
    case 32:
        r->x5 = arg[0];
        break;
    }
}

void func_800604EC(void)
{
#include "../shared/func_8001B530.inc.c"
}

void func_800604F4(Ctx0x80060460 *ctx)
{
    AreaTbl *t;
    unsigned char *arg;
    unsigned char *e;

    t = WORK->areas;
    e = (unsigned char *)t + t->offs[ctx->x9F];
    arg = ctx->arg;
    if (((int *)arg)[1] == 30) {
        e[3] = arg[0];
    }
}



extern void func_800484F0(Unit *, int, int, int, int);


extern unsigned char D_800B49D7;


extern void func_80041DC0(int, int);


#define SHARED_F0 func_80034D88

extern void func_80021EF0(int a, int b);


#define SHARED_F0 func_8002EB14

extern void func_8004A4D4(Unit *, int, int, int);


extern int func_80034CD8(int, int);
extern void func_80051F8C(int, int, int, int, int);


extern int func_80035A88(int a, int b);



extern void (*D_8008A3B0[])(Elem28 *);


extern void func_800484F0(Unit *, int, int, int, int);


extern int func_80081CB4(void);


extern int func_8001C680(int a, int b);


#define SHARED_F0 func_80041994

extern void func_80037068(int);
extern void func_80034C48(int, int, int);
extern void func_8002EB14(int, int, int);
extern int D_800B4A60;
extern int D_800B4A64;


extern void func_800371A0(void);
extern int func_80034CD8(int, int);
extern void func_8002EB14(int, int, int);
extern int D_800B4A60;
extern int D_800B4A64;



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
extern int func_80065D0C(int, int, char *, int);
extern char D_8008EB88[];
extern char D_8008E98E[];


extern int D_800B4F74;



extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;




#define SHARED_D0 D_8008F780




#define SHARED_D0 D_8008F78C

#define SHARED_F0 func_800661FC

