#include "common.h"

typedef struct {
    unsigned char pad0[0x48];
    short f48;
    short f4A;
    short f4C;
    unsigned char pad4E[0x7B - 0x4E];
    unsigned char f7B;
    unsigned char pad7C[0x98 - 0x7C];
    short f98;
    short f9A;
    short f9C;
} Obj0x8002b7e0;

void func_8002B7E0(Obj0x8002b7e0 *p)
{
    p->f7B = 0;
    p->f48 = p->f98;
    p->f4A = p->f9A;
    p->f4C = p->f9C;
}
