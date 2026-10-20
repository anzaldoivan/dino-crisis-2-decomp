#include "common.h"

#include "dc2.h"
extern void func_800428D0();
extern void func_80047B8C(void *, void *);

void func_8002B2C8(Unit *p)
{
    int t;

    t = p->x42;
    t = t / -1000;
    p->x108 = 1000;
    p->flags |= 2;
    p->x5A = t;
    func_800428D0(p);
    *(int *)&p->x1C0 = 0;
    func_80047B8C(p, (void *)p->x60);
    p->xFE = 3;
    ((unsigned char *)&p->x54)[0] = 1;
    ((unsigned char *)&p->x54)[1] = 0;
}
