#include "common.h"

#include "dc2.h"
extern int (*D_80088E70[])(void);

void func_8002BFD8(void)
{
    int i = WORK->xC50 >> 8;
    D_80088E70[i]();
}
