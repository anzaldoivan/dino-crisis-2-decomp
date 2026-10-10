#include "dc2.h"

typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5;
    unsigned short h6, h8, hA, hC, hE;
} Ent0x80028b78;
typedef struct {
    unsigned char a0, a1, a2, a3;
    unsigned short s4, s6, s8, sA, sC, sE, s10, s12;
} Rec0x80028b78;
typedef struct {
    char pad0[0x2438];
    unsigned char x2438;
    unsigned char x2439;
    unsigned char x243A;
    unsigned char x243B;
    char pad243C[4];
    char x2440[0xC];
    Rec0x80028b78 x244C;
    char pad2460[0x2481 - 0x2460];
    unsigned char x2481;
} Obj0x80028b78;
typedef struct {
    char pad0[0xC];
    Rec0x80028b78 xC;
    char pad20[0x364 - 0x20];
    unsigned char x364;
} Work0x80028b78;
#define WORK0x80028b78 (*SCRATCH(Work0x80028b78 *, 0))
extern Ent0x80028b78 D_80088B48[];
extern void func_80079904();
extern void *func_80081CA4();
extern void func_80037C1C();

void func_80028B78(Obj0x80028b78 *obj)
{
    Ent0x80028b78 *e;
    Rec0x80028b78 *r;

    e = &D_80088B48[obj->x2481];
    obj->x2438 = e->b0;
    obj->x2439 = e->b1;
    obj->x243A = e->b2;
    obj->x243B = 1;
    func_80079904(obj->x2438, obj->x2439, obj->x243A);
    func_80081CA4(obj->x2440, 0, 12);
    r = &obj->x244C;
    r->a3 = 1;
    r->a0 = e->b3;
    r->a1 = e->b4;
    r->a2 = e->b5;
    r->s4 = e->h6;
    r->s6 = e->h8;
    r->s8 = e->hA;
    r->sA = e->hC;
    r->sE = e->hE;
    WORK0x80028b78->xC = obj->x244C;
    WORK0x80028b78->x364 = 0;
    func_80037C1C();
}
