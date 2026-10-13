#include "common.h"

typedef struct Obj0x80029bb0 {
    char pad0[0x248C];
    int f248C;
} Obj0x80029bb0;
extern void func_800296E4();
extern void func_8002980C();

void func_80029BB0(Obj0x80029bb0 *obj)
{
    func_800296E4(0xBE, 0xC8, 0x80, 0xD0, 0, 0x808080);
    func_8002980C(obj->f248C, 0xF6, 0xD2, 7, 0x808080);
}
