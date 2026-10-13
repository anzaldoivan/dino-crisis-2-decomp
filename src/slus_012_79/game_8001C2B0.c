#include "common.h"
#include "dc2.h"

extern unsigned char D_800A79BA[];
extern int D_800AF11C;
extern void (*D_80085FA8[])(Ent0x8001c2b0 *);
extern int func_80034CD8(int, int);

void func_8001C2B0(void)
{
    Unit *u;
    Ent0x8001c2b0 *e;

    D_800A79BA[1] = 0;
    D_800A79BA[0] = 0;
    if (WORK->units != 0) {
        for (u = WORK->units; u < WORK->units + D_800AF11C; u++) {
            if (u->flags & 1) {
                for (e = (Ent0x8001c2b0 *)((char *)u + 0x190); e < (Ent0x8001c2b0 *)((char *)u + 0x1C0); e++) {
                    if (e->flags & 1) {
                        D_80085FA8[(e->flags >> 1) & 1](e);
                    }
                }
            }
        }
    }
    for (e = (Ent0x8001c2b0 *)((char *)WORK + 0x4EC); e < (Ent0x8001c2b0 *)((char *)WORK + 0x5DC); e++) {
        if (e->flags & 1) {
            D_80085FA8[(e->flags >> 1) & 1](e);
        }
    }
    if (func_80034CD8(2, 4)) {
        D_800A79BA[1] = 0;
        D_800A79BA[0] = 0;
    }
}
