#include "common.h"

void func_8002B7E0(unsigned char *arg0);

void func_8002B9D4(unsigned char *arg0)
{
    unsigned short v;

    func_8002B7E0(arg0);
    v = *(unsigned short *)(arg0 + 0x9E);
    *(unsigned char *)(arg0 + 0x7B) = 4;
    *(short *)(arg0 + 0x9E) = 0;
    *(short *)(arg0 + 0x9C) = 0;
    *(short *)(arg0 + 0x98) = 0;
    *(short *)(arg0 + 0x9A) = v;
}
