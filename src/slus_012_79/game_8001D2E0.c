#include "common.h"

#include "dc2.h"
typedef Sub0x8001d28c Obj0x8001d2e0;
typedef struct {
    char pad0[4];
    unsigned char x4;
    char pad5[0x2C - 5];
} Elem0x8001d2e0;

Elem0x8001d2e0 *func_8001D2E0(Obj0x8001d2e0 *a0, Elem0x8001d2e0 *a1)
{
    unsigned short i;
    unsigned int n = WORK->x5E8;

    for (i = 0; i < n; i++, a1++) {
        if (a1->x4 == a0->x4C) {
            break;
        }
    }
    return a1;
}
