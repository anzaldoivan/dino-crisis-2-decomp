#include "dc2.h"

extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern void func_8001DB64();
extern void func_8001DBFC(void);
extern void func_8002EB14(int, int, int);
extern void func_8004262C(int);
extern void func_8002F398();
extern int D_800A79F0;
extern short D_800ABA68;
extern short D_800ABA90;
#define SPAD08_0x8002fa34 (*SCRATCH(short, 0x8))
#define SPAD0A_0x8002fa34 (*SCRATCH(short, 0xA))

void func_8002FA34(void)
{
    Work *w = WORK;
    unsigned char *p;
    int old;

    if (w->x30C == 0) {
        w->x30C++;
        func_8004262C(1);
        return;
    }
    w->x6E9 = 1;
    p = (unsigned char *)w->x318;
    old = p[0x4C];
    p[0x4C] = p[0x4E];
    WORK->x3E7 = 0xFF;
    func_8001DB64(old);
    func_8001DBFC();
    WORK->x6E8 = 0;
    func_8002EB14(1, 1, 1);
    func_8001CDE4(1);
    func_8002EB14(0, 15, 1);
    *(int *)((char *)WORK->units + 0x54) = 1;
    if (WORK->xC2D == 7) {
        WORK->x6F8 = 0x70;
        WORK->x6F9 = 0x70;
        WORK->x6FA = 0x70;
        WORK->x6FB = 0;
        WORK->x6FC = 0;
        D_800A79F0 = 3;
    } else {
        D_800A79F0 = 2;
    }
    D_800ABA68 = 0;
    SPAD08_0x8002fa34 = 0;
    SPAD0A_0x8002fa34 = 0;
    D_800ABA90 = 0;
    WORK->x30C = 0;
    func_8001CED4((int)func_8002F398);
}
