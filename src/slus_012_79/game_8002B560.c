#include "common.h"
#include "dc2.h"

typedef struct Part0x8002b560 {
    char pad0[0x58];
    struct Part0x8002b560 *x58;
    char pad5c[0xCC - 0x5C];
} Part0x8002b560;
typedef struct {
    char pad0[0x60];
    Part0x8002b560 *x60;
} Obj0x8002b560;
extern int D_800ABA6C;
extern int D_800AF11C;

Unit *func_8002B560(Obj0x8002b560 *a0)
{
    Part0x8002b560 *p = a0->x60;
    Part0x8002b560 *base = *(Part0x8002b560 **)WORK->x4D8;

    if (p >= base && !(base + D_800ABA6C < p)) {
        while ((Unit *)p < WORK->units || WORK->units + D_800AF11C < (Unit *)p) {
            p = p->x58;
        }
        return (Unit *)p;
    }
    return 0;
}
