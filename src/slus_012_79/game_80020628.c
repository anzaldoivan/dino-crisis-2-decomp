#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0xCB];
    unsigned char xCB;
} Sub0x80020628;
extern unsigned short D_800196C8[];
extern int func_80034CD8(int, int);

void func_80020628(Unit *obj)
{
    Sub0x80020628 *s;
    int n;

    n = D_800196C8[obj->x58];
    s = (Sub0x80020628 *)WORK->x22C;
    if (func_80034CD8(9, 0x36)) {
        n <<= 1;
    }
    if (*(signed char *)WORK->padc2a == 0) {
        n <<= 1;
    }
    s->xCB = 1;
    WORK->xD90 += n;
    WORK->x7B4 += n;
    WORK->xDA8 += n;
    if (WORK->xD90 > 9999999) {
        WORK->xD90 = 9999999;
    }
}
