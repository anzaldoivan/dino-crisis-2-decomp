#include "dc2.h"

typedef struct Work0x8002ebe8 {
    char pad0[0x3A0];
    unsigned char x3A0;
} Work0x8002ebe8;
#define WORK0x8002ebe8 (*SCRATCH(Work0x8002ebe8 *, 0))
typedef struct Obj0x8002ebe8 {
    unsigned char pad0[0x12];
    unsigned char state;
} Obj0x8002ebe8;
extern void (*D_800891F0[])(Obj0x8002ebe8 *);

void func_8002EBE8(void)
{
    Obj0x8002ebe8 *obj = (Obj0x8002ebe8 *)&WORK0x8002ebe8->x3A0;
    D_800891F0[obj->state](obj);
}
