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








INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005EDA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005EE58);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005EF48);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005EF8C);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F024);


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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F1EC);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F410);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F45C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_8005F4BC);

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
extern void func_80079AA4();

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_80060460);

void func_800604EC(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005EDA0", func_800604F4);



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
extern void func_80065D0C(int, int, char *, int);
extern char D_8008EB88[];
extern char D_8008E98E[];


extern int D_800B4F74;



extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;




#define SHARED_D0 D_8008F780




#define SHARED_D0 D_8008F78C

#define SHARED_F0 func_800661FC

