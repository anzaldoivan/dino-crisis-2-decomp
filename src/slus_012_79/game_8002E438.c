#include "common.h"
#include "dc2.h"

typedef struct Tmr0x8002e438 {
    char pad0[0x8];
    short x8;
} Tmr0x8002e438;
typedef struct Obj0x8002e438 {
    char pad0[0x40];
    unsigned short x40;
    unsigned short x42;
    unsigned short x44;
    char pad46[0x6C - 0x46];
    unsigned short x6C;
    unsigned short x6E;
    char pad70[0x7A - 0x70];
    unsigned char x7A;
    char pad7B[0x7C - 0x7B];
    unsigned short x7C;
    unsigned short x7E;
    unsigned short x80;
    char pad82[0x8C - 0x82];
    Sub0x8002c024 *x8C;
    char pad90[0x98 - 0x90];
    Tmr0x8002e438 x98;
} Obj0x8002e438;
extern void func_80047814(Obj0x8002e438 *o);
extern int func_80048274(Obj0x8002e438 *o);

void func_8002E438(Obj0x8002e438 *o)
{
    Sub0x8002c024 *s;
    Tmr0x8002e438 *t;

    s = o->x8C;
    o->x40 += o->x7C;
    o->x42 += o->x7E;
    o->x44 += o->x80;
    if ((unsigned)(o->x7A - 3) < 2) {
        t = &o->x98;
        if (t->x8 != 0) {
            t->x8--;
        } else {
            if (o->x6E) {
                o->x6E--;
            }
            if (o->x6C) {
                o->x6C--;
            } else {
                if (s->x1 == 1) {
                    s->x18--;
                }
                func_80047814(o);
            }
        }
    }
    if (func_80048274(o)) {
        if (s->x1 == 1) {
            s->x18--;
        }
        func_80047814(o);
    }
}
