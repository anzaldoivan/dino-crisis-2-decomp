#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned int tag;
    unsigned char r0;
    unsigned char g0;
    unsigned char b0;
    unsigned char code;
    short x0;
    short y0;
    unsigned char u0;
    unsigned char v0;
    unsigned short clut;
    short w;
    short h;
} Sprt0x800296e4;
typedef struct {
    unsigned int *ot;
} OtRef0x800296e4;
extern OtRef0x800296e4 D_800ABA98;
extern void func_8007E074();
extern void func_8007E194();

void func_800296E4(short x, short y, short u, short v, unsigned char small, int color)
{
    Sprt0x800296e4 s;
    unsigned int *p;
    unsigned int *ot;

    *(int *)&s.r0 = color;
    s.code = 0x64;
    s.x0 = x;
    s.y0 = y;
    if (small) {
        s.w = 8;
        s.h = 8;
    } else {
        s.w = 0x60;
        s.h = 0x10;
    }
    s.u0 = u;
    s.v0 = v;
    s.clut = 0x7DCB;
    *(Sprt0x800296e4 *)PKT = s;
    p = PKT;
    ot = D_800ABA98.ot;
    ((Sprt0x800296e4 *)p)->tag = ot[31] | 0x04000000;
    ot[31] = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x14);
    func_8007E194(PKT, 1, 0, 0x29, 0);
    func_8007E074(&D_800ABA98.ot[31], PKT);
    PKT = (unsigned int *)((unsigned char *)PKT + 0xC);
}
