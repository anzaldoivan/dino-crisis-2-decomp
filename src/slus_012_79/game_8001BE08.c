#include "common.h"
#include "dc2.h"

extern int D_800AF11C;

void func_8001BE08(void)
{
    Unit *u;
    Elem0x8001be08 *e;

    u = WORK->units;
    while (u < WORK->units + D_800AF11C) {
        if (u->flags & 1) {
            for (e = u->x190; e < &u->x190[2]; e++) {
                if (e->flags & 1) {
                    e->flags &= ~1;
                }
            }
        }
        u++;
    }
    for (e = WORK->x4EC; e < &WORK->x4EC[10]; e++) {
        if (e->flags & 1) {
            e->flags &= ~1;
        }
    }
}
