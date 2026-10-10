#include "common.h"

typedef struct Sub0x8002b380 {
    int value;
    unsigned short count;
} Sub0x8002b380;
typedef struct Obj0x8002b380 {
    unsigned char pad0[0x50];
    int flags;
    unsigned char pad54[0x1C0 - 0x54];
    Sub0x8002b380 sub;
} Obj0x8002b380;
extern int func_800487B8(Obj0x8002b380 *);

void func_8002B380(Obj0x8002b380 *obj)
{
    Sub0x8002b380 *sub = &obj->sub;

    sub->value = func_800487B8(obj);
    if (--sub->count == 0xFFFF) {
        obj->flags |= 0x40000000;
    }
}
