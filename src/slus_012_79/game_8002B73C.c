#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x40];
    unsigned short x40;
    unsigned short x42;
    unsigned short x44;
} Obj0x8002b73c;
typedef struct {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    short x4;
    short x6;
    short x8;
    short xA;
    short xC;
    short xE;
} Sub0x8002b73c;
extern void func_8002B614();
extern void func_8002B41C();
extern const unsigned short D_800183F8[];

void func_8002B73C(Obj0x8002b73c *o, unsigned char idx)
{
    Work *w = WORK;
    Sub0x8002b73c *p;

    func_8002B614(o);
    func_8002B41C(o);
    p = (Sub0x8002b73c *)w->padC;
    p->x3 = 1;
    p->x4 = o->x40;
    p->x6 = o->x42;
    p->x8 = o->x44;
    p->xA = 0x600;
    p->xE = D_800183F8[idx];
    p->x0 = 0x80;
    p->x1 = 0x80;
    p->x2 = 0x80;
}
