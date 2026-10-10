#include "dc2.h"

extern void func_80018020(void *, int);
extern int func_80034CD8(int, int);

Ent0x8001c478 *func_8001C478(void)
{
    Ent0x8001c478 *p;
    Ent0x8001c478 *end;

    if (func_80034CD8(2, 4)) {
        return 0;
    }
    p = (Ent0x8001c478 *)((char *)WORK + 0x4EC);
    end = p + 10;
    for (; p < end; p++) {
        if (!(p->flags & 1)) {
            func_80018020(p, 6);
            p->flags |= 1;
            return p;
        }
    }
    return 0;
}
