#include "common.h"

typedef struct {
    short f0;
    short f2;
    short f4;
    short f6;
} Sub0x8002c76c;
typedef struct {
    char pad0[0x48];
    short f48;
    char pad4a[0x55 - 0x4a];
    unsigned char f55;
    char pad56[0x7b - 0x56];
    unsigned char f7b;
    short f7c;
    short f7e;
    short f80;
    char pad82[0x98 - 0x82];
    Sub0x8002c76c s98;
} Obj0x8002c76c;
extern int func_8004A32C(void);

void func_8002C76C(Obj0x8002c76c *o)
{
    Sub0x8002c76c *s = &o->s98;

    s->f2--;
    s->f6 -= 20;
    if (s->f6 < 30) {
        s->f6 = 30;
    }
    if (o->f7e > 40) {
        o->f7e = 80;
        o->f7b = 0;
        o->f7c >>= 1;
        o->f80 >>= 1;
        o->f55++;
        o->f48 &= 0xfff;
        if (o->f48 > 0x800) {
            o->f55++;
        }
        s->f0 = 0x30;
        s->f2 = func_8004A32C();
        s->f4 = func_8004A32C();
        s->f6 = 0;
    }
}
