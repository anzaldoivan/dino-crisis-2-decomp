#include "common.h"

typedef struct Obj0x8001e358 {
    unsigned char pad0[0x6A];
    unsigned char state;
} Obj0x8001e358;
extern Obj0x8001e358 D_800AF5C0;
extern void (*D_800875A4[])(Obj0x8001e358 *);

void func_8001E358(void)
{
    Obj0x8001e358 *obj = &D_800AF5C0;
    D_800875A4[obj->state](obj);
}
