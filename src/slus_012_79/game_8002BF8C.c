#include "common.h"

#include "dc2.h"
extern int (*D_80088E3C[])(void);

void func_8002BF8C(void)
{
    int i = WORK->xC50 & 0xF;
    D_80088E3C[i]();
}
