#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x4C];
    unsigned short x4C;
    char pad4E[0x7E - 0x4E];
    unsigned short x7E;
    char pad80[0x8C - 0x80];
    Sub0x8002b874 *x8C;
    char pad90[0x98 - 0x90];
    unsigned short x98;
    unsigned short x9A;
    short x9C;
} Obj0x8002b874;
extern void func_80047814(Obj0x8002b874 *);

void func_8002B874(Obj0x8002b874 *obj)
{
    Sub0x8002b874 *sub;

    sub = obj->x8C;
    obj->x7E += obj->x98;
    obj->x4C += obj->x9A;
    if (--obj->x9C == 0) {
        func_80047814(obj);
        if (sub != 0 && sub->x1 == 1) {
            sub->x18--;
        }
    }
}
