#include "dc2.h"

typedef struct {
    char pad0[0x4C7];
    unsigned char x4C7;
} Work0x8002df30;
#define WORK0x8002df30 (*SCRATCH(Work0x8002df30 *, 0))
typedef struct {
    short x0;
    short x2;
    short pad4;
    short x6;
    short pad8;
    short xA;
} Sub0x8002df30;
typedef struct {
    char pad0[0x20];
    MATRIX x20;
    char pad40[0x48 - 0x40];
    short x48;
    char pad4a[0x50 - 0x4A];
    int x50;
    unsigned char x54;
    char pad55[0x67 - 0x55];
    unsigned char x67;
    short x68;
    short x6A;
    short x6C;
    short x6E;
    char pad70[0x7A - 0x70];
    unsigned char x7A;
    char pad7b[0x84 - 0x7B];
    unsigned char x84;
    char pad85[0x98 - 0x85];
    Sub0x8002df30 x98;
} Obj0x8002df30;
extern void func_80047814();
extern void func_80048248();
extern int func_80079AA4();
extern void *func_80081CA4();

void func_8002DF30(Obj0x8002df30 *o)
{
    Sub0x8002df30 *s = &o->x98;
    SVECTOR v;

    func_80081CA4(&v, 0, 8);
    o->x54++;
    if (WORK0x8002df30->x4C7 != 0) {
        o->x6A = 0x2B;
        func_80047814(o);
    } else {
        o->x6A = 0xB;
    }
    o->x68 = 0x7DC0;
    func_80079AA4(&v, &o->x20);
    o->x48 = 0x400;
    o->x50 |= 2;
    o->x67 |= 2;
    switch (o->x7A) {
    case 0:
    case 1:
        o->x6C = 0;
        o->x6E = 0;
        break;
    case 2:
        o->x6C = o->x6E = s->x2;
        break;
    }
    o->x84 |= 1;
    func_80048248(o, D_80181E28);
    s->xA = 0;
    if (WORK0x8002df30->x4C7 != 0) {
        if (s->x6 == 0) {
            s->x6 = 3;
        }
        s->x6 = s->x6 * 30;
    } else {
        s->x6 = 0;
    }
}
