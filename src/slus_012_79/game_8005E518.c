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





extern void func_8002EB14(int, int, int);
extern int func_80034CD8(int, int);
extern void func_8004262C(int);


extern void func_8002EB14(int, int, int);
extern int func_80034CD8(int, int);
extern void func_8004262C(int);


/* func_8005E518, func_8005E6A4 (T8 w36: above both) */
typedef struct State0x8005e6a4 {
    char pad0[4];
    unsigned char x4;
} State0x8005e6a4;
extern int *D_800B49D0;        /* word 0 is a pointer (func_8005EAB4); was extern State0x8005e6a4 */
/* func_8005E854, func_8005E8F8 (T8 w36: above both) */
typedef struct {
    char pad[0xA4];
} Elem0x8005e8f8;
extern Elem0x8005e8f8 *D_800B49C8;
extern void func_80018020(void *, int);

typedef struct State0x8005e518 {
    char pad0[4];
    unsigned char x4;
    char pad5[7 - 5];
    unsigned char x7;
    char pad8[0xA - 8];
    unsigned short xA;
} State0x8005e518;
extern unsigned short D_800ABA90;
extern void (*D_8008A35C[])(void);
#define S0x8005e518 ((State0x8005e518 *)&D_800B49D0)

void func_8005E518(void) {
    if (D_800ABA90 & 0x100) {
        if (S0x8005e518->x7 != 0xFF && func_80034CD8(2, 6) == 0) {
            S0x8005e518->x4 = 1;
        }
    }
    D_8008A35C[S0x8005e518->x4]();
    S0x8005e518->xA++;
    WORK->x6C4 = 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005E518", func_8005E5BC);

extern unsigned int D_800AF5B8;
extern void func_8001BE08();
extern void func_8001D9E4(void);
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern void func_80034D88();
extern void func_80047584(Elem0x8005e6a4 *e);
extern void func_8005E8F8(signed char i);
extern void func_80065EC8(void);
extern void func_80065EFC(void);

void func_8005E6A4(void)
{
    unsigned int i;
    Elem0x8005e6a4 *e;

    func_8001D9E4();
    func_80034D88(-1, 0, 0);
    func_80034C48(2, 0xC, 0);
    for (i = 0; i < 10; i++) {
        func_8005E8F8(i);
    }
    func_80065EC8();
    func_80065EFC();
    func_80034C48(2, 0x1F, 0);
    for (i = 0; i < D_800AF5B8; i++) {
        e = &((Elem0x8005e6a4 *)WORK->x4E8)[i];
        if ((e->x0 & 1) && e->x1 == 1) {
            func_80047584(e);
        }
    }
    func_8001BE08();
    func_8002EB14(1, 5, 1);
    ((State0x8005e6a4 *)&D_800B49D0)->x4++;
}

extern int *D_800B49D0;
extern int func_80034CD8(int, int);
void func_8005E854(signed char i, short off);

void func_8005E7D8(void)
{
    if (func_80034CD8(2, 6) == 0) {
        WORK->x6B2 = 2;
        WORK->x6B0 = 0xFF;
        func_8005E854(0, S0x8005e518->x7);
        S0x8005e518->x7 = 0xFF;
        S0x8005e518->x4 = 0;
    }
}

extern int D_8008A358;

int func_8005E844(void) {
    return D_8008A358 + 0x10;
}

typedef struct Elem0x8005e854 {
    char *x0;
    char pad4[0x80 - 0x4];
    int x80;
    int *x84;
    char pad88[0x9B - 0x88];
    unsigned char x9B;
    char pad9C[0xA0 - 0x9C];
    unsigned char xA0;
    char padA1[0xA4 - 0xA1];
} Elem0x8005e854;

void func_8005E854(signed char i, short off)
{
    int *p;
    Elem0x8005e854 *e;
    char *v;

    p = (int *)WORK->x5F4;
    e = (Elem0x8005e854 *)&D_800B49C8[i];
    p += off;

    func_80018020(e, 0x29);
    e->x9B = 1;
    v = WORK->x5F4 + *p;
    e->x84 = &e->x80;
    e->xA0 = 0xFF;
    e->x0 = v;
}


void func_8005E8F8(signed char i)
{
    func_80018020(&D_800B49C8[i], 0x29);
}

typedef struct Obj0x8005e940 {
    char pad0[0x9E];
    unsigned char kind;
    signed char idx;
} Obj0x8005e940;

void *func_8005E940(Obj0x8005e940 *o)
{
    switch (o->kind) {
    case 0:
        return (char *)WORK->units + o->idx * 0x260;
    case 1:
        return (char *)WORK->x4DC + o->idx * 0x70;
    case 2:
        return (char *)WORK->x4E4 + o->idx * 0xAC;
    case 3:
        return (char *)WORK->x4E8 + o->idx * 0x34;
    }
    return 0;
}

extern int *D_800B49D0;

int func_8005EA34(Ctx8005EEF4 *ctx, unsigned char kind, int v)
{
    switch (kind) {
    case 0:
    case 3:
    case 4:
    case 5:
        break;
    case 1:
        v = ctx->regs[v];
        break;
    case 2:
        v = ctx->sp[v];
        break;
    case 6:
        v = D_800B49D0[v];
        break;
    }
    return v;
}

typedef Ctx8005EEF4 Ctx0x8005eab4;
extern int *D_800B49D0;

int func_8005EAB4(Ctx0x8005eab4 *ctx, unsigned char kind, int i, int v)
{
    switch (kind) {
    case 1:
        ctx->regs[i] = v;
        break;
    case 2:
        ctx->sp[i] = v;
        break;
    case 6:
        D_800B49D0[i] = v;
        break;
    }
    return v;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8005E518", func_8005EB34);

struct Ctx0x8005eccc;

void func_8005EC80(struct Ctx0x8005eccc *ctx, int v) {
    if (v == 0) {
        ((unsigned char *)ctx)[0x9C] = 0x2A;
        return;
    }
    if (v > 0) {
        ((unsigned char *)ctx)[0x9C] = 0x4C;
        return;
    }
    if (v < 0) {
        ((unsigned char *)ctx)[0x9C] = 0x70;
    }
}

int func_8005ECB4(int *a0)
{
#include "../shared/func_8005ECB4.inc.c"
}

typedef struct Ctx0x8005eccc {
    unsigned char *pc;
    char pad04[0x88 - 0x4];
    int regs[1];
} Ctx0x8005eccc;
extern int func_8005EA34(Ctx8005EEF4 *ctx, unsigned char kind, int v);
extern void func_8005EC80(Ctx0x8005eccc *ctx, int v);

int func_8005ECCC(Ctx0x8005eccc *ctx)
{
    int v;

    v = func_8005EA34((Ctx8005EEF4 *)ctx, ctx->pc[2], *(short *)(ctx->pc + 4));
    ctx->regs[ctx->pc[1]] = v;
    func_8005EC80(ctx, v);
    ctx->pc += 6;
    return 0;
}

extern int func_8005EAB4(Ctx8005EEF4 *ctx, unsigned char a, int b, int c); /* was void (.., int a, ..) (T10 w39 draft) */





extern void func_80034C48(int, int, int);






#define SHARED_F0 func_80034D88

#define SHARED_F0 func_8002EB14


extern void (*D_8008A3B0[])(Elem28 *);


#define SHARED_F0 func_80041994


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

