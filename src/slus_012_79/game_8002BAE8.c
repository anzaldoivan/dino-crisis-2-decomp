#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x48];
    unsigned short x48;
    unsigned short x4A;
    unsigned short x4C;
    char pad4E[0x8C - 0x4E];
    Sub0x8002b874 *x8C;
    char pad90[0x98 - 0x90];
    unsigned short x98;
    unsigned short x9A;
    unsigned short x9C;
    short x9E;
} Obj0x8002bae8;
extern void func_80047814(Obj0x8002bae8 *);

void func_8002BAE8(Obj0x8002bae8 *obj)
{
    Sub0x8002b874 *sub;

    sub = obj->x8C;
    obj->x48 = obj->x98;
    obj->x4A = obj->x9A;
    obj->x4C = obj->x9C;
    if (--obj->x9E == 0) {
        func_80047814(obj);
        if (sub != 0 && sub->x1 == 1) {
            sub->x18--;
        }
    }
}
