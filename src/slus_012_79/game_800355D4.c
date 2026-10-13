#include "common.h"
#include "dc2.h"

typedef struct Ent0x800355d4 {
    char pad0[0x50];
    int x50;
    char pad54[0x58 - 0x54];
    unsigned char x58;
    char pad59[0xAC - 0x59];
} Ent0x800355d4;
extern unsigned int D_800AF590;
extern int (**D_800AF124)();

void func_800355D4(void)
{
    Ent0x800355d4 *e;
    int n;

    n = D_800AF590;
    e = (Ent0x800355d4 *)WORK->x4E4;
    while (n--) {
        if (e->x50 & 1) {
            D_800AF124[e->x58](e);
        }
        e++;
    }
}
