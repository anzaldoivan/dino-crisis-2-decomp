#include "dc2.h"

extern void func_80034C48(int, int, int);

void func_80022400(void)
{
    Sub22C *s = (Sub22C *)((char *)WORK + 0x22C);

    s->xB8 = 0;
    s->xC8 = 0;
    func_80034C48(3, 7, 0);
}
