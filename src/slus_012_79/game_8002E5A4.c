#include "common.h"

typedef struct Obj0x8002e5a4 {
    unsigned char pad0[0x54];
    unsigned char state;
    unsigned char pad55[0x6C - 0x55];
    unsigned short f6C;
    unsigned short f6E;
    unsigned char pad70[0xA2 - 0x70];
    unsigned short fA2;
} Obj0x8002e5a4;
extern int func_80048274(Obj0x8002e5a4 *o);

void func_8002E5A4(Obj0x8002e5a4 *obj)
{
    func_80048274(obj);
    obj->f6E = obj->f6C = (short)--obj->fA2 >> 4;
    if (obj->f6E == 0) {
        obj->state++;
    }
}
