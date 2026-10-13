#include "common.h"

typedef struct Obj0x80029b4c {
    char pad0[0x248A];
    unsigned short f248A;
} Obj0x80029b4c;
extern void func_800296E4();
extern void func_8002980C();

void func_80029B4C(Obj0x80029b4c *obj)
{
    func_800296E4(0xBE, 0xBB, 0x80, 0xC0, 0, 0x808080);
    func_8002980C(obj->f248A, 0x11E, 0xBB, 2, 0x808080);
}
