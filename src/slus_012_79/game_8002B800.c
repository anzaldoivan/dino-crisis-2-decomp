#include "common.h"

void func_8002B800(unsigned char *a0)
{
    *(unsigned char *)(a0 + 0x7B) = 0;
    *(unsigned short *)(a0 + 0x4A) = *(unsigned short *)(a0 + 0x98);
}
