#include "common.h"
#include "dc2.h"

typedef struct { unsigned char b[8]; } Name8;
typedef struct {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    unsigned short x4;
    unsigned char x6;
    unsigned char pad7;
    Name8 x8;
    char pad10[0x16 - 0x10];
    unsigned short x16;
    char pad18[0x20 - 0x18];
    Name8 x20;
} Rec;
typedef struct {
    char *base;
    int x4;
    unsigned char x8;
    unsigned char pad9;
    unsigned short xA;
} Hdr;
typedef struct {
    char pad0[0xC];
    Hdr hdr;
} Src;
typedef struct {
    char pad0[0x40];
    Name8 x40;
    Name8 x48;
    char pad50[0x58 - 0x50];
    unsigned char x58;
    char pad59[0x60 - 0x59];
    int x60;
    short x64;
    unsigned char x66;
    unsigned char x67;
    int x68;
    short x6C;
    short x6E;
    char *x70;
    char pad74[0x84 - 0x74];
    unsigned char x84;
} Obj80026504;
extern unsigned char D_80087DEC[];
extern unsigned char D_80087DDC[];
extern int func_800474B4(int a, int b);
extern Obj80026504 *func_80047714(void);
extern void func_8002B614(Obj80026504 *o);
extern void func_8002B41C(Obj80026504 *o);

void func_80026504(Src *src, Rec *rec)
{
    Hdr *h = &src->hdr;
    char *base = h->base;
    Obj80026504 *o;

    while (*(int *)rec != -1 && (o = func_80047714()) != 0) {
        o->x58 = 15;
        o->x6C = rec->x1;
        o->x6E = rec->x2;
        o->x64 = rec->x4;
        o->x66 = rec->x6;
        o->x40 = rec->x8;
        o->x40.b[6] = 0;
        o->x84 = rec->x3;
        o->x68 = h->x4;
        o->x70 = base + rec->x16;
        o->x48 = rec->x20;
        o->x67 |= D_80087DEC[h->xA];
        rec++;
        o->x60 = func_800474B4(WORK->units->id, D_80087DDC[h->x8 - 1]);
        func_8002B614(o);
        func_8002B41C(o);
    }
}
