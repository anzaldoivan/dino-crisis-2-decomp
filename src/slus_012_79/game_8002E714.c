#include "common.h"

#include "dc2.h"
typedef struct {
    u8 pad0[0x54];
    u8 unk54;
    u8 pad55[0x64 - 0x55];
    u8 unk64;
    u8 unk65;
    u8 unk66;
} Obj8002E714;

void func_8002E714(Obj8002E714 *p)
{
    p->unk64 += 16;
    p->unk65 += 16;
    p->unk66 += 16;
    if (p->unk64 > 0x50) {
        p->unk54--;
    }
}
