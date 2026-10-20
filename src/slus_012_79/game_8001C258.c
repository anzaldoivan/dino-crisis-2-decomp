#include "common.h"

typedef struct {
    char pad0[0xC];
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int b3 : 13;
    unsigned int cnt : 8;
    unsigned int b24 : 8;
    char pad10[3];
    unsigned char x13;
} Obj0x8001c258;
extern unsigned char D_800A79BA[];

void func_8001C258(Obj0x8001c258 *o)
{
    D_800A79BA[o->b2] = o->x13;
    if (--o->cnt == 0xFF) {
        o->b0 = 0;
    }
}
