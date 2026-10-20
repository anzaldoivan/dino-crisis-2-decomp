#include "common.h"

void func_800205EC(void)
{
#include "../shared/func_8001B530.inc.c"
}

#include "dc2.h"
typedef struct {
    char pad0[0xC0];
    short xC0;
    short xC2;
    char padC4[0xCA - 0xC4];
    char xCA;
    char xCB;
    char padCC[0xCE - 0xCC];
    char xCE;
    char xCF;
} Sub0x800205f4;

void func_800205F4(void)
{
    Sub0x800205f4 *s = (Sub0x800205f4 *)WORK->x22C;

    s->xCA = 0;
    s->xC0 = 0;
    s->xCB = 0;
    s->xC2 = 0;
    s->xCE = 0;
    s->xCF = 0;
}

void func_80020620(void)
{
#include "../shared/func_8001B530.inc.c"
}
