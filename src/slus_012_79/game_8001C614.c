#include "common.h"

extern unsigned char *func_8001C510(void);
extern unsigned char *func_8001C478(void);

unsigned char *func_8001C614(int sel, int a, int b)
{
    unsigned char *p;

    if (sel != 0) {
        p = func_8001C510();
    } else {
        p = func_8001C478();
    }
    if (p != 0) {
        p[0x13] = b;
        *(int *)(p + 0xC) |= 7;
        p[0xE] = a;
    }
    return p;
}
