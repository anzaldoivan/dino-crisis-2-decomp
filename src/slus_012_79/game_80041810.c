#include "dc2.h"

extern Grp0x80029458 D_80089B08[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);

void func_80041810(void)
{
    Elem0x80029458 *e;

    e = &D_80089B08[WORK->xC2D].xC[WORK->xC2C];
    if (WORK->x21E != e->x2) {
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        func_8001EC10(e->x2);
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        WORK->x21E = e->x2;
    }
}
