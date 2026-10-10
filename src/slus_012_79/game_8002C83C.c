#include "common.h"
#include "dc2.h"

int func_8004A32C(void);
extern int func_80078844(int);
extern int func_80078914(int);

void func_8002C83C(Obj0x8002c83c *p)
{
    short *s;
    Sub0x8002c83c *q;

    p->f7c = (short)(p->f7c * 2) / 3;
    p->f80 = (short)(p->f80 * 2) / 3;
    s = p->f98;
    q = p->f8c;
    p->f40 += func_80078844(s[1]) >> 8;
    p->f44 += func_80078914(s[2]) >> 8;
    s[1] += (((unsigned int)func_8004A32C() >> 3) & 0xFF) + 16;
    s[2] += (((unsigned int)func_8004A32C() >> 3) & 0xFF) + 16;
    s[1] &= 0xFFF;
    s[2] &= 0xFFF;
    p->f48 += s[0];
    if (p->f48 < 0x300) {
        s[0] = 0x30;
    } else if (p->f48 > 0x500) {
        s[0] = -0x30;
    }
    if (p->f42 > s[6]) {
        p->f54++;
        q->f18--;
    }
}
