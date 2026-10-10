#include "common.h"

typedef struct Obj0x8004a4d4 {
    unsigned char pad0[0x110];
    short x;
    short y;
    unsigned char pad114[4];
    unsigned char flags;
} Obj0x8004a4d4;

void func_8004A4D4(Obj0x8004a4d4 *obj, int flags, int x, int y) {
    obj->flags = flags | 0x80;
    obj->x = x;
    obj->y = y;
}
