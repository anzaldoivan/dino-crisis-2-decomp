#include "common.h"
#include "dc2.h"

extern unsigned int *D_800ABA98;
extern void func_80023AFC();

void func_80023564(int x, int y, unsigned short n, unsigned short lead, unsigned int col, unsigned int val)
{
    unsigned short i;
    unsigned short d;
    unsigned int div;
    Box0x8002243c box;
    unsigned int *ot;
    unsigned int *p;
    unsigned int rgb;
    unsigned int hi;

    div = 1;
    rgb = col & 0xFFFFFF;
    hi = col >> 31;
    for (i = 1; i < n; i++) {
        div *= 10;
    }
    for (i = 0; i < n; i++) {
        d = val / div;
        if (d) {
            lead = 1;
        }
        val -= d * div;
        div /= 10;
        if (div == 0) {
            lead = 1;
        }
        if (lead || d) {
            box.x = x + i * 7;
            box.y = y;
            box.w = 8;
            box.h = 12;
            func_80023AFC(&box, (short)(d * 8), 0x18, 0x7C8F, rgb, hi);
        }
    }
    PKT[1] = 0xE100040C;
    PKT[2] = 0;
    ot = D_800ABA98;
    p = PKT;
    *p = ot[31] | 0x2000000;
    ot[31] = (unsigned int)p & 0xFFFFFF;
    PKT = PKT + 3;
}
