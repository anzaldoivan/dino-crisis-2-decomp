#include "common.h"

#include "dc2.h"
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
} Prim0x80029fc4;
typedef struct {
    void *x0;
    unsigned int *pkt;
} Spad0x80029fc4;
extern OtHolder0x800310b8 *D_800ABA98;
extern unsigned char D_800ABA96;
extern void func_8007E194();

void func_80029FC4(unsigned int *arg0)
{
    unsigned int x;
    unsigned int y;
    unsigned int col;
    unsigned int tp;
    OtHolder0x800310b8 *ot;
    unsigned int *p;

    ((unsigned char *)PKT)[3] = 2;
    PKT[1] = 0xE6000000;
    PKT[2] = 0;
    {
        OtHolder0x800310b8 *o = D_800ABA98;
        unsigned int *q = PKT;
        *q = (*q & 0xFF000000) | (o->x78 & 0xFFFFFF);
        o->x78 = (o->x78 & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    }
    PKT += 3;
    col = (*arg0 & 0xFFFFFF) | 0x66000000;
    for (x = 0; x < 0x140; x += 0x40) {
        for (y = 0; y < 0xF0; y += 0x30) {
            ((short *)PKT)[4] = x;
            ((short *)PKT)[5] = y;
            ((unsigned char *)PKT)[12] = x & 0x3F;
            ((unsigned char *)PKT)[13] = y;
            ((short *)PKT)[8] = 0x40;
            ((short *)PKT)[9] = 0x30;
            ((short *)PKT)[7] = 0x7DB0;
            *(unsigned int *)&((Prim0x80029fc4 *)PKT)->r0 = col;
            ot = D_800ABA98;
            p = PKT;
            *p = ot->x78 | 0x04000000;
            ot->x78 = (unsigned int)p & 0xFFFFFF;
            PKT += 5;
        }
        tp = (((unsigned char *)arg0)[3] << 5) | 0x100;
        if (D_800ABA96 == 0) {
            tp |= (x >> 6) + 0x10;
        } else {
            tp |= x >> 6;
        }
        func_8007E194(SCRATCH(Spad0x80029fc4, 0)->pkt, 1, 1, tp, 0);
        {
            OtHolder0x800310b8 *o = D_800ABA98;
            unsigned int *q = PKT;
            *q = (*q & 0xFF000000) | (o->x78 & 0xFFFFFF);
            o->x78 = (o->x78 & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
        }
        PKT += 3;
    }
    ((unsigned char *)PKT)[3] = 2;
    *(unsigned int *)&((Prim0x80029fc4 *)PKT)->r0 = 0xE6000001;
    PKT[2] = 0;
    {
        OtHolder0x800310b8 *o = D_800ABA98;
        unsigned int *q = PKT;
        *q = (*q & 0xFF000000) | (o->x78 & 0xFFFFFF);
        o->x78 = (o->x78 & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    }
    PKT += 3;
}
