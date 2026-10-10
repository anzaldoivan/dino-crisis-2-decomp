#include "common.h"
#include "dc2.h"

extern unsigned char D_800ABA96;
extern OtHolder0x8002329c *D_800ABA98;
extern void func_8007C10C();

void func_8002329C(int x, short y, short w, short h)
{
    Rect0x8006c2d8 r;
    OtHolder0x8002329c *o;
    unsigned int *p;

    r.x = x;
    r.y = D_800ABA96 ? y + 0x100 : y;
    r.w = w;
    r.h = h;
    func_8007C10C(PKT, &r);
    p = PKT;
    o = D_800ABA98;
    *p = (*p & 0xFF000000) | (o->ot & 0xFFFFFF);
    o->ot = (o->ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT = PKT + 3;
}
