#include "common.h"
#include "dc2.h"

extern int func_80034CD8(int, int);
extern void func_80034D88();
extern void func_80079924(int, int);

void func_80030670(void)
{
    Work *w;
    Work *w2;
    Ramp0x80030670 *r;
    unsigned int d;
    int t;

    if (func_80034CD8(2, 12) != 0) {
        w = WORK;
        w->x712 ^= 1;
        WORK->x7C6 = WORK->x7C7;
        w2 = WORK;
        r = &w->x6EC;
        if (w2->x712 != 0) {
            w2->x7C7 = 0;
        } else {
            d = 1;
            if (r->x3 != 0) {
                d = r->x3;
            }
            t = w->x6EC.x0 * (d - r->x2);
            w2->x7C7 = ((r->x1 * r->x2 + t) << 16) / d >> 16;
        }
        func_80079924(0xA0, WORK->x7C7 + 0x78);
        if (r->x3 != 0) {
            if (++r->x2 == r->x3) {
                func_80034D88(-1, 0, 0);
            }
        }
    }
}
