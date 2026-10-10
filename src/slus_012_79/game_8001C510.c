#include "common.h"
#include "dc2.h"

typedef struct Obj0x8001c510 {
    char pad0[0x190];
    Ent0x8001c478 ents[2];
} Obj0x8001c510;
extern void func_80018020(void *, int);
extern int func_80034CD8(int, int);

Ent0x8001c478 *func_8001C510(Obj0x8001c510 *obj)
{
    Ent0x8001c478 *p;

    if (func_80034CD8(2, 4)) {
        return 0;
    }
    for (p = obj->ents; p < &obj->ents[2]; p++) {
        if (!(p->flags & 1)) {
            func_80018020(p, 6);
            p->flags |= 1;
            return p;
        }
    }
    return 0;
}
