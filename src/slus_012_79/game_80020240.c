#include "common.h"
#include "dc2.h"

/* Blk0x80020240: dc2.h (T8 w35) */
typedef struct {
    int pad0;
    Blk0x80020240 *dst;
} Ctx0x80020240;
typedef struct {
    unsigned char pad0[0x69];
    unsigned char idx;
    unsigned char pad6a[0x70 - 0x6A];
    unsigned short state;
} Obj0x80020240;
typedef struct {
    int pad0;
    int size;
    int off;
    unsigned short padc;
    unsigned short slot;
} Arg0x80020240;
extern Ctx0x80020240 D_800B32F0;
extern int D_800B2BA0[];
extern Blk0x80020240 *D_80087564[];
extern void func_80081C94();

void func_80020240(Obj0x80020240 *obj, Arg0x80020240 *arg)
{
    Ctx0x80020240 *c = &D_800B32F0;

    if (obj->state == 0) {
        D_800B2BA0[arg->slot] = arg->off + 0x801F2900;
        c->dst = (Blk0x80020240 *)(arg->off + 0x801F2900);
        obj->state++;
    }
    if (arg->size > 0x800) {
        *c->dst = *D_80087564[obj->idx];
    } else {
        func_80081C94(c->dst, D_80087564[obj->idx], arg->size);
    }
    c->dst = (Blk0x80020240 *)((char *)c->dst + 0x800);
}
