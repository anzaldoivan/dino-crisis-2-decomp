#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned short x0;
    short x2;
    short x4;
    char pad6[0xA - 0x6];
    short xA;
} Timer0x8002e098;
typedef struct {
    char pad0[0x54];
    unsigned char x54;
    char pad55[0x6C - 0x55];
    unsigned short x6C;
    unsigned short x6E;
    char pad70[0x7A - 0x70];
    unsigned char x7A;
    char pad7b[0x98 - 0x7B];
    Timer0x8002e098 x98;
} Obj0x8002e098;

void func_8002E098(Obj0x8002e098 *obj)
{
    Timer0x8002e098 *t = &obj->x98;

    if (obj->x6E < t->x2 && obj->x7A != 2) {
        t->xA++;
        if (t->x4 < t->xA) {
            t->xA = 0;
            obj->x6E += t->x0;
            obj->x6C += t->x0;
        }
        return;
    }
    t->xA = 0;
    if (WORK->x4C7) {
        obj->x54++;
    }
}
