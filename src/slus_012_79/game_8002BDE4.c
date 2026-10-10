#include "common.h"

extern unsigned char D_80088E38;

int func_8002BDE4(unsigned char arg0)
{
    if (arg0 != 0) {
        D_80088E38 = arg0;
        return 0;
    }
    if (D_80088E38 != 0) {
        arg0 = D_80088E38;
        D_80088E38 = 0;
    }
    return arg0;
}
