#include "common.h"

#include "dc2.h"
typedef struct Obj8002C484 {
    unsigned char pad0[0x64];
    unsigned char c64;
    unsigned char c65;
    unsigned char c66;
    unsigned char pad67[0x6C - 0x67];
    unsigned short t6C;
    unsigned short t6E;
    unsigned char pad70[0x98 - 0x70];
    short t98;
} Obj8002C484;
void func_80047814(Obj8002C484 *p);
void func_80048274(Obj8002C484 *p);

void func_8002C484(Obj8002C484 *p)
{
    if (--p->t98 < 0) {
        p->c64 -= 2;
        p->c65 -= 2;
        p->c66 -= 2;
        if (p->c64 == 0) {
            func_80047814(p);
        }
        if (p->t6C != 0) {
            p->t6C--;
        }
        if (p->t6E != 0) {
            p->t6E--;
        }
        if (p->t6C * p->t6E == 0) {
            func_80047814(p);
        }
    }
    func_80048274(p);
}
