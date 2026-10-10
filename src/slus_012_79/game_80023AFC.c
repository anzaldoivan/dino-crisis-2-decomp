#include "dc2.h"

extern OtHolder0x8002329c *D_800ABA98;

void func_80023AFC(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe)
{
    unsigned int *p;
    OtHolder0x8002329c *h;

    ((Sprt0x8006c454 *)PKT)->x = rect[0];
    ((Sprt0x8006c454 *)PKT)->y = rect[1];
    ((Sprt0x8006c454 *)PKT)->w = rect[2];
    ((Sprt0x8006c454 *)PKT)->h = rect[3];
    ((Sprt0x8006c454 *)PKT)->u = u;
    ((Sprt0x8006c454 *)PKT)->v = v;
    ((Sprt0x8006c454 *)PKT)->clut = clut;
    if (abe) {
        ((Sprt0x8006c454 *)PKT)->code = color | 0x66000000;
    } else {
        ((Sprt0x8006c454 *)PKT)->code = color | 0x64000000;
    }
    h = D_800ABA98;
    p = PKT;
    *p = h->ot | 0x04000000;
    h->ot = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x14);
}
