#include "common.h"

typedef struct Obj {
    char pad0[0x48];
    short x;
    short y;
    short z;
    short pad4E;
    int flags;
    char pad54[0x10];
    int id;
} Obj;
typedef struct Work {
    char pad0[0x4E0];
    Obj *obj;
} Work;
extern Obj *func_800474B4(int a, int b);

void func_800D9000(Obj *dst) {
    Obj *src = func_800474B4((*(Work **)0x1F800000)->obj->id, 7);
    src->flags |= 0x10000000;
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
    dst->flags |= 2;
}
