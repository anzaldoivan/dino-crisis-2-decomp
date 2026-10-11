#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned char pad0[0x69];
    unsigned char idx;
} Obj0x8001fdf8;
typedef struct {
    int pad0;
    int size;
    Blk0x80020240 *dst;
} Arg0x8001fdf8;
extern Blk0x80020240 *D_80087564[];
extern void func_80081C94();

void func_8001FDF8(Obj0x8001fdf8 *obj, Arg0x8001fdf8 *arg)
{
    if (arg->size > 0x800) {
        *arg->dst = *D_80087564[obj->idx];
    } else {
        func_80081C94(arg->dst, D_80087564[obj->idx], arg->size);
    }
    arg->dst = (Blk0x80020240 *)((char *)arg->dst + 0x800);
}
