#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0xB4];
    int xB4;
    char padB8[0xCA - 0xB8];
    unsigned char xCA;
} Sub0x80020724;
extern int func_80034CD8(int, int);

void func_80020724(Unit *obj, unsigned short n)
{
    Sub0x80020724 *s;

    s = (Sub0x80020724 *)WORK->x22C;
    if (n != 0) {
        if (obj->x58 == 8 && *(unsigned short *)&WORK->xC2C == 0x808) {
            n = 100;
        }
        if (func_80034CD8(9, 0x36)) {
            n <<= 1;
        }
        if (*(signed char *)WORK->padc2a == 0) {
            n <<= 1;
        }
        s->xCA = 1;
        WORK->xD90 += n;
        WORK->x7B4 += n;
        WORK->xDA8 += n;
        s->xB4 = n;
        if (WORK->xD90 > 9999999) {
            WORK->xD90 = 9999999;
        }
    }
}
