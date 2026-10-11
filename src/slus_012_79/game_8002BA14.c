#include "common.h"

typedef struct {
    char pad0[0x6C];
    short x6C;
    short x6E;
    char pad70[0xB];
    unsigned char x7B;
    char pad7C[0x10];
    unsigned char *x8C;
    char pad90[8];
    short x98;
    short x9A;
    short x9C;
} Obj0x8002ba14;
extern void func_80047814();

void func_8002BA14(Obj0x8002ba14 *p)
{
    unsigned char *s = p->x8C;

    if (--p->x98 < 0) {
        p->x6E += p->x9A;
        if (p->x6E < 0) {
            goto kill;
        }
        if (p->x6E >= 0x100) {
            p->x6E = 0xFF;
            p->x7B = 0;
        }
        p->x6C += p->x9C;
        if (p->x6C < 0) {
        kill:
            func_80047814(p);
            if (s != 0 && s[1] == 1) {
                s[0x18]--;
            }
        } else if (p->x6C >= 0x100) {
            p->x6C = 0xFF;
            p->x7B = 0;
        }
    }
}
