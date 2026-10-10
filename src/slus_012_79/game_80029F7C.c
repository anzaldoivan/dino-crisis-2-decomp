#include "dc2.h"

typedef struct Obj0x80029f7c {
    unsigned char pad0[4];
    unsigned char state;
} Obj0x80029f7c;
extern void (*D_80088DB4[])(Obj0x80029f7c *);

void func_80029F7C(void)
{
    Obj0x80029f7c *obj = (Obj0x80029f7c *)&WORK->x6F8;
    D_80088DB4[obj->state](obj);
}
