#include "dc2.h"

typedef struct {
    unsigned char flags;
    char pad1[0x13];
    int x14;
} Obj0x80064460;
extern unsigned char *func_80065D0C(int, int, char *, int);

void func_80064460(Obj0x80064460 *obj)
{
    unsigned char *p;

    p = func_80065D0C(0x1E, 0xBE, WORK->x5F4 + obj->x14, 3);
    if ((obj->flags & 0xC) == 8) {
        *p |= 2;
    }
}
