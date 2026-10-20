#include "common.h"

#include "dc2.h"
extern int D_800AF150;
extern int (**D_800ABAE8)(Shot *);
void func_8007AA34(SVECTOR *, MATRIX *);

void func_80035744(void)
{
    Shot *p;

    for (p = WORK->x4DC; p < WORK->x4DC + D_800AF150; p++) {
        if (p->x50 & 1) {
            D_800ABAE8[p->x4E](p);
            func_8007AA34((SVECTOR *)&p->x48, &p->m);
        }
    }
}
