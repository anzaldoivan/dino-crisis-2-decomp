#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x86];
    unsigned char x86;
    unsigned char x87;
    char pad88[0xAC - 0x88];
} Ent0x8002b6d0;

void func_8002B6D0(Ent0x8002b6d0 *e)
{
    e->x86 = WORK->x3EE;
    WORK->x3EE = e->x87;
    if (e->x86 != 0xFF) {
        ((Ent0x8002b6d0 *)WORK->x4E4)[e->x86].x87 = e->x87;
    }
}
