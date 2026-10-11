#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned int ot;
    int x4;
    int x8;
} Ent0x80034e24;
extern unsigned char D_800ABA96;
extern unsigned int *D_800ABA98;
extern Ent0x80034e24 D_800ABA80[];
extern void func_8007E0B4();
extern void func_8007E194();

void func_80034E24(void)
{
    func_8007E194(PKT, 1, 0, 0x29, 0);
    ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)&D_800ABA80[D_800ABA96])->addr;
    ((Tag0x8004c960 *)&D_800ABA80[D_800ABA96])->addr = (unsigned int)PKT;
    PKT += 3;
    func_8007E0B4(D_800ABA98 + 28, &D_800ABA80[D_800ABA96], &D_800ABA80[D_800ABA96 - 1].x4);
}
