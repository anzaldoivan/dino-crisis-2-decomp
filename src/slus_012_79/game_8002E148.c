#include "common.h"

#include "dc2.h"
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} Sub_s8002E148;
typedef struct {
    u8 pad0[0x54];
    u8 unk54;
    u8 pad55[0x98 - 0x55];
    Sub_s8002E148 sub;
} Obj_s8002E148;

void func_8002E148(Obj_s8002E148 *p)
{
    Sub_s8002E148 *q = &p->sub;

    if (q->unk6 < ++q->unkA) {
        p->unk54++;
    }
}
