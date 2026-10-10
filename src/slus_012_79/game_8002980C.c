#include "dc2.h"

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} Sprt0x8002980c;
extern unsigned int *D_800ABA98[];
extern void func_8007E194();
extern void func_8007E074();

void func_8002980C(unsigned int num, short x, short y, unsigned char ndig, int rgb)
{
    Sprt0x8002980c s;
    unsigned int div;
    int flag;
    int i;
    int q;
    unsigned int *ot;
    unsigned int *p;

    flag = 1;
    i = 1;
    div = 1;
    s.v0 = 0;
    s.y0 = y;
    s.w = 8;
    s.h = 0x10;
    s.clut = 0x7C8F;
    *(int *)&s.r0 = rgb;
    s.code = 0x64;
    for (; i < ndig; i++) {
        div *= 10;
    }
    x -= 8;
    for (i = 0; i < ndig; i++) {
        x += 8;
        q = num / div;
        if (div == 1) {
            flag = 0;
        }
        if (q != 0 || flag == 0) {
            s.x0 = x;
            s.u0 = q * 8;
            *(Sprt0x8002980c *)PKT = s;
            p = PKT;
            {
                unsigned int *o = D_800ABA98[0];
                *p = o[31] | 0x4000000;
                o[31] = (unsigned int)p & 0xFFFFFF;
            }
            PKT += 5;
            flag = 0;
            num -= q * (int)div;
        }
        div /= 10;
    }
    if (ndig == 7) {
        s.x0 = x + 8;
        s.w = 0x10;
        s.u0 = 0x50;
        *(Sprt0x8002980c *)PKT = s;
        ot = D_800ABA98[0];
        p = PKT;
        p[0] = ot[31] | 0x4000000;
        ot[31] = (unsigned int)p & 0xFFFFFF;
        PKT += 5;
    }
    func_8007E194(PKT, 1, 0, 0x2C, 0);
    func_8007E074(D_800ABA98[0] + 31, PKT);
    PKT += 3;
}
