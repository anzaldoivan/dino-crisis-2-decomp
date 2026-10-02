#include "common.h"
#include "dc2.h"

typedef struct {
    u8 pad0[0x54];
    u8 unk54;
    u8 pad55[0x64 - 0x55];
    u8 unk64;
    u8 unk65;
    u8 unk66;
} Obj8002E18C;

void func_8002E18C(Obj8002E18C *p) {
    p->unk64 -= 8;
    p->unk65 -= 8;
    p->unk66 -= 8;
    if (p->unk64 == 0) {
        p->unk54++;
    }
}
