#include "common.h"
#include "dc2.h"

extern int func_80034CD8(int, int);
extern int (**D_800AE074)(Unit *);
extern int D_800AF11C;

void func_800354A8(void)
{
    Unit *u;
    int i;

    if (func_80034CD8(1, 3) == 0) {
        u = WORK->units;
        if (u->flags & 1) {
            D_800AE074[u->x58](u);
            u->x48.vy &= 0xFFF;
        }
    }
    if (func_80034CD8(1, 2) == 0) {
        u = WORK->units + 1;
        for (i = D_800AF11C - 2; i != -1; i--, u++) {
            if (u->flags & 1) {
                D_800AE074[u->x58](u);
                u->x48.vy &= 0xFFF;
            }
        }
    }
}
