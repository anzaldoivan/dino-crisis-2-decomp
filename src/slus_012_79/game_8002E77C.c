#include "common.h"

typedef struct Sub0x8002e77c {
    unsigned char x0;
} Sub0x8002e77c;
typedef struct Obj0x8002e77c {
    char pad0[0x46];
    unsigned char x46;
    char pad47[0x50 - 0x47];
    int x50;
    unsigned char x54;
    char pad55[0x60 - 0x55];
    int x60;
    char pad64[0x7B - 0x64];
    unsigned char x7B;
    char pad7C[0x84 - 0x7C];
    unsigned char x84;
    char pad85[0x8C - 0x85];
    Sub0x8002e77c *x8C;
} Obj0x8002e77c;
extern void (*D_80088E10[])(Obj0x8002e77c *);
extern void (*D_800891E4[])(Obj0x8002e77c *);
extern void func_8002B41C(Obj0x8002e77c *o);
extern void func_8002B614(Obj0x8002e77c *o);
extern void func_80047814(Obj0x8002e77c *o);
extern void func_8004A66C(Obj0x8002e77c *o);

void func_8002E77C(Obj0x8002e77c *o)
{
    Sub0x8002e77c *s;

    s = o->x8C;
    D_80088E10[o->x7B](o);
    D_800891E4[o->x54](o);
    func_8002B614(o);
    if (o->x50 & 4) {
        func_8004A66C(o);
    }
    if (o->x8C && !(s->x0 & 1) && !(o->x84 & 8)) {
        func_80047814(o);
    }
    if (o->x46 && --o->x46 == 0 && o->x60) {
        func_8002B41C(o);
    }
}
