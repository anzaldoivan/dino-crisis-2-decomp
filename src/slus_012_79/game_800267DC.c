#include "common.h"

#include "dc2.h"
extern int func_80034CD8(int, int);
extern void func_80044834(void *, int);
extern void func_800484F0();
extern void func_80079AA4();

void func_800267DC(Obj0x80026a7c *p)
{
    Unit0x80026a7c *u = WORK0x80026a7c->x4E0;
    Unit0x80026a7c *v;
    Sub0x80026a7c *s;
    unsigned int c;

    switch (p->x2477) {
    case 1:
        if (func_80034CD8(2, 6) == 0) {
            c = WORK0x80026a7c->xC2B;
            if (c == 2) {
                p->x2477 = 3;
                p->x247C = c;
            } else {
                p->x2477++;
                func_800484F0(u, WORK0x80026a7c->xC2B ? 2 : 3, 0, 0x18, 10);
            }
        }
        break;
    case 2:
        if (u->x92 < 11) {
            break;
        }
        p->x2477 = 3;
        func_80044834(0, 0x78);
    case 3:
        v = WORK0x80026a7c->x4E0;
        s = (Sub0x80026a7c *)((char *)v + 0x260);
        s->x4A += 0x14;
        func_80079AA4(v->x2A8, v->x280);
        c = u->x92;
        c = c < 0x1D;
        if (c == 0) {
            p->x2477 += 2;
        }
        break;
    }
}
