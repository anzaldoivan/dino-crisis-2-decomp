#include "common.h"

typedef struct Obj0x8002e54c {
    unsigned char pad0[0x54];
    unsigned char state;
    unsigned char pad55[0x6E - 0x55];
    unsigned short t6E;
    unsigned char pad70[0x9E - 0x70];
    short t9E;
    unsigned char padA0[0xA2 - 0xA0];
    unsigned short tA2;
} Obj0x8002e54c;
int func_80048274(Obj0x8002e54c *p);

void func_8002E54C(Obj0x8002e54c *p)
{
    func_80048274(p);
    if (--p->t9E == 0) {
        p->tA2 = p->t6E << 4;
        p->state++;
    }
}
