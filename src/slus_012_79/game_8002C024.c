#include "common.h"
#include "dc2.h"

typedef struct Obj0x8002c024 {
    char pad0[0x46];
    unsigned char x46;
    char pad47[0x50 - 0x47];
    int x50;
    unsigned char x54;
    char pad55[0x60 - 0x55];
    struct Obj0x8002c024 *x60;
    char pad64[0x7A - 0x64];
    unsigned char x7A;
    unsigned char x7B;
    char pad7C[0x84 - 0x7C];
    unsigned char x84;
    char pad85[0x8C - 0x85];
    Sub0x8002c024 *x8C;
} Obj0x8002c024;
extern void (*D_80088E10[])(Obj0x8002c024 *);
extern void (*D_80088F18[])(Obj0x8002c024 *);
extern void func_8002B41C(Obj0x8002c024 *o);
extern void func_8002B614(Obj0x8002c024 *o);
extern void func_80047814(Obj0x8002c024 *o);
extern void func_8004A66C(Obj0x8002c024 *o);

void func_8002C024(Obj0x8002c024 *o)
{
    Sub0x8002c024 *s;

    s = o->x8C;
    D_80088E10[o->x7B](o);
    D_80088F18[o->x54](o);
    if (o->x7A == 15 && o->x60 && !(o->x60->x50 & 1)) {
        func_80047814(o);
        if (s->x1 == 1) {
            s->x18--;
        }
    } else {
        func_8002B614(o);
    }
    if (((s->x0 & 1) + (o->x84 & 8)) == 0 && s->x1 == 1) {
        func_80047814(o);
    }
    if (o->x50 & 4) {
        func_8004A66C(o);
    }
    if (o->x46 && --o->x46 == 0) {
        func_8002B41C(o);
    }
}
