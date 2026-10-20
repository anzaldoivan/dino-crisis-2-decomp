#include "common.h"

#include "dc2.h"
extern unsigned int *D_800ABA98;
extern void func_80023AFC();

void func_80023990(Spr0x800317fc *p, int x, int y, int tpage, int clut, unsigned int color)
{
    int n;
    unsigned int *ot;
    unsigned int *p2;

    n = p->n;
    while (n-- != 0) {
        Rect0x8006c2d8 r2 = { p->x + x, p->y + y, p->w, p->h };
        ((void (*)(Rect0x8006c2d8 *, unsigned char, unsigned char, unsigned short, unsigned int, unsigned int))func_80023AFC)(&r2, p->u, p->v, clut, color & 0xFFFFFF, color >> 31);
        p++;
    }
    PKT[1] = tpage | 0xE1000400;
    PKT[2] = 0;
    ot = &D_800ABA98[31];
    p2 = PKT;
    *p2 = *ot | 0x02000000;
    *ot = (unsigned int)p2 & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0xC);
}
