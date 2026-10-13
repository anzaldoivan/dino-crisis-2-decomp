#include "common.h"

typedef struct Obj0x80029aec {
    char pad0[0x2484];
    int f2484;
} Obj0x80029aec;
extern void func_800296E4();
extern void func_8002980C();

void func_80029AEC(Obj0x80029aec *obj)
{
    func_800296E4(0xBE, 0xA4, 0, 0xD0, 0, 0x8000);
    func_8002980C(obj->f2484, 0xF6, 0xAE, 7, 0x8000);
}
