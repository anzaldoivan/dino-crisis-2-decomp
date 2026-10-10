#include "dc2.h"

extern int func_80034CD8(int, int);
extern void func_8001D338(Sub0x8001d230 *, int);

void func_8001D230(void)
{
    Sub0x8001d230 *s = (Sub0x8001d230 *)WORK->x318;

    if (func_80034CD8(1, 10) == 0) {
        func_8001D338(s, WORK->x5E4);
    }
    s->x4E = s->x4C;
}
