#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x50];
    int x50;
    unsigned char x54;
    char pad55[0x60 - 0x55];
    int x60;
    char pad64[0x67 - 0x64];
    unsigned char x67;
    short x68;
    short x6A;
    short x6C;
    short x6E;
    char pad70[0x84 - 0x70];
    unsigned char x84;
} Obj0x8002e650;
extern void func_80048248();

void func_8002E650(Obj0x8002e650 *o)
{
    o->x6A = 0x2B;
    o->x68 = 0x7DC3;
    o->x54++;
    func_80048248(o, D_80181E48);
    o->x6C = 1;
    o->x6E = 1;
    o->x60 = 0;
    o->x50 |= 2;
    o->x67 |= 2;
    o->x84 |= 0x80;
}
