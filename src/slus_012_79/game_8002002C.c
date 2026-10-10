#include "common.h"

typedef struct {
    char pad0[0x69];
    unsigned char x69;
    char pad6a[0x70 - 0x6A];
    unsigned short x70;
} Obj0x8002002c;
typedef struct {
    char pad0[4];
    int x4;
    char pad8[0xE - 0x8];
    unsigned short xE;
} Arg0x8002002c;
typedef struct {
    char b[0x800];
} Blk0x8002002c;
extern char *D_800B32F0;
extern char *D_8008A034[];
extern char *D_80087564[];
extern void func_80081C94();

void func_8002002C(Obj0x8002002c *obj, Arg0x8002002c *arg)
{
    char **p = &D_800B32F0;

    if (obj->x70 == 0) {
        *p = D_8008A034[arg->xE];
        obj->x70++;
    }
    if (arg->x4 > 0x800) {
        *(Blk0x8002002c *)*p = *(Blk0x8002002c *)D_80087564[obj->x69];
    } else {
        func_80081C94(*p, D_80087564[obj->x69], arg->x4);
    }
    *p += 0x800;
}
