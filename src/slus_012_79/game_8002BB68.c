#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x50];
    int x50;
    char pad54[0x58 - 0x54];
    unsigned char x58;
    char pad59[0x8C - 0x59];
    unsigned char *x8C;
    char pad90[0xAC - 0x90];
} Ent0x8002bb68;
extern unsigned int D_800AF590;
extern void func_80047814();
extern void func_8002641C();

void func_8002BB68(unsigned char arg0)
{
    int i;
    unsigned int n;
    Ent0x8002bb68 *e;

    n = D_800AF590;
    i = n - WORK->x610;
    e = (Ent0x8002bb68 *)WORK->x4E4 + (n - 1);
    while (i-- != 0) {
        if ((e->x50 & 1) && e->x58 == arg0) {
            func_80047814(e);
            if (e->x8C != 0) {
                *e->x8C = 0;
            }
        }
        e--;
    }
    if ((unsigned int)(arg0 - 1) < 2) {
        WORK->xF = 0;
        if ((WORK->xC50 & 0xF) != 5) {
            func_8002641C();
        }
    }
}
