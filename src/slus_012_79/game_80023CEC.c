#include "dc2.h"

extern unsigned int *D_800ABA98;

void func_80023CEC(short x, short y, unsigned short w, unsigned short h, unsigned int color, unsigned short abe)
{
    unsigned int *p;
    unsigned int *ot;

    ((Tile0x80023cec *)PKT)->x = x;
    ((Tile0x80023cec *)PKT)->y = y;
    ((Tile0x80023cec *)PKT)->w = w;
    ((Tile0x80023cec *)PKT)->h = h;
    if (abe) {
        ((Tile0x80023cec *)PKT)->code = color | 0x42000000;
    } else {
        ((Tile0x80023cec *)PKT)->code = color | 0x40000000;
    }
    ot = &D_800ABA98[31];
    p = PKT;
    *p = *ot | 0x03000000;
    *ot = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x10);
}
