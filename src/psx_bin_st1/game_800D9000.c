#include "common.h"
#include "dc2.h"

typedef struct Obj800D9000 {
    char pad0[0x48];
    short x;
    short y;
    short z;
    short pad4E;
    int flags;
    char pad54[0x10];
    int id;
} Obj800D9000;
extern Obj800D9000 *func_800474B4(int a, int b);

void func_800D9000(Obj800D9000 *dst) {
    Obj800D9000 *src = func_800474B4(WORK->units->id, 7);
    src->flags |= 0x10000000;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->flags |= 2;
}
