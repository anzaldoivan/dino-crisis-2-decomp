#include "common.h"

#include "dc2.h"
extern int D_800AF5B8;
extern int (**D_800AF118)(Elem0x8005e6a4 *);

void func_80035670(void)
{
    Elem0x8005e6a4 *p;

    for (p = WORK->x4E8; p < (Elem0x8005e6a4 *)WORK->x4E8 + D_800AF5B8; p++) {
        if (p->x0 & 1) {
            D_800AF118[p->x1](p);
        }
    }
}
