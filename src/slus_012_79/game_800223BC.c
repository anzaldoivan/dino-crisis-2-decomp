#include "common.h"

#include "dc2.h"
extern void func_80034C48(int, int, int);

void func_800223BC(void)
{
    Sub22C *s = (Sub22C *)((char *)WORK + 0x22C);

    s->xB8 = 0x78;
    s->xC8 = 1;
    func_80034C48(3, 7, 1);
}
