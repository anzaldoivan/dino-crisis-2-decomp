#include "common.h"
#include "dc2.h"

extern void func_80034C48(int, int, int);
extern void func_80079924(int, int);

void func_80034D88(int a0, int a1, int a2)
{
    Ramp0x80030670 *r;

    if (a0 < 0) {
        func_80034C48(2, 12, 0);
        WORK->x7C7 = 0;
        WORK->x713 = 1;
        func_80079924(0xA0, 0x78);
    } else {
        r = &WORK->x6EC;
        r->x0 = a0;
        r->x1 = a1;
        r->x2 = 0;
        r->x3 = a2;
        WORK->x712 = 1;
        func_80034C48(2, 12, 1);
    }
}
