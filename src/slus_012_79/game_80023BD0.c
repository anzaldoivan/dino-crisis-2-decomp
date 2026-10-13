#include "common.h"
#include "dc2.h"

extern unsigned int *D_800ABA98;

void func_80023BD0(short *rect, short u, short v, short clut, int color, unsigned short abe)
{
    unsigned int *ot;
    unsigned int *p;

    ((short *)PKT)[4] = ((short *)PKT)[12] = rect[0];
    ((short *)PKT)[8] = ((short *)PKT)[16] = rect[0] + 8;
    ((short *)PKT)[5] = ((short *)PKT)[9] = rect[1];
    ((short *)PKT)[13] = ((short *)PKT)[17] = rect[1] + 11;
    ((unsigned char *)PKT)[12] = ((unsigned char *)PKT)[28] = u;
    ((unsigned char *)PKT)[20] = ((unsigned char *)PKT)[36] = u + 8;
    ((unsigned char *)PKT)[13] = ((unsigned char *)PKT)[21] = v;
    ((unsigned char *)PKT)[29] = ((unsigned char *)PKT)[37] = v + 11;
    ((short *)PKT)[7] = clut;
    ((short *)PKT)[11] = 12;
    if (abe) {
        PKT[1] = color | 0x2E000000;
    } else {
        PKT[1] = color | 0x2C000000;
    }
    ot = D_800ABA98;
    p = PKT;
    *p = ot[31] | 0x09000000;
    ot[31] = (unsigned int)p & 0xFFFFFF;
    PKT += 10;
}
