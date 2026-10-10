#include "dc2.h"

extern int D_800A79F0;
extern void func_80018020(void *, int);
extern void func_8001B538();
extern void func_8001D1E4();
extern void func_8001D230();
extern void func_80023D8C();
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern void func_80034D88();
extern int func_80035A88(int a, int b);
extern void func_80037C5C();
extern void func_8003C688();
extern void func_80041810();
extern void func_800418DC();
extern void func_80041AE4();
extern void func_800454BC();
extern int func_8004587C();
extern void func_80049D10();
extern int func_8004B778();
extern void func_8005E450();
extern void func_80064A60();

void func_8002F654(void)
{
    Work *w;
    void *u;
    int v;

    WORK->x204++;
    if (WORK->x4C6 == 0) {
        func_80049D10();
    }
    func_8001B538(0);
    func_80018020(WORK->x4D8, 0xBC);
    D_800A79F0 = 2;
    if (*(signed char *)WORK->padc2a == 0) {
        func_80034C48(2, 0x1A, 1);
    }
    if (WORK->x4C7 == 1) {
        switch (WORK->x3EC) {
        case 0:
        case 1:
        case 5:
        case 6:
            break;
        default:
            func_80034C48(2, 0x1A, 1);
            break;
        }
    }
    func_80041810();
    func_80034D88(-1, 0, 0);
    func_80041AE4(D_800E0000);
    WORK->x629 = 0xFF;
    WORK->x671 = 0xFF;
    if (WORK->x4C7 == 0) {
        v = func_8004587C();
        if (v != 0xFFFF) {
            func_800418DC((unsigned short)v);
        }
    }
    WORK->x6EB = 4;
    func_80034C48(3, 1, 1);
    func_80034C48(3, 2, 1);
    func_80034C48(3, 3, 1);
    func_80034C48(3, 4, 1);
    func_80034C48(3, 5, 1);
    func_80034C48(3, 8, 1);
    w = WORK;
    u = w->units;
    switch (w->x4C7) {
    case 1:
        if (w->xC2B == 3) {
            func_80034C48(7, 0x11, 1);
        }
    case 0:
        func_8003C688(u);
        break;
    case 2:
        func_80034C48(2, 0x21, 1);
        break;
    }
    if (WORK->x202 != 0) {
        WORK->x202 = 0;
        func_80034C48(2, 0x23, 1);
    }
    func_800454BC();
    func_8001D1E4();
    func_80064A60();
    func_8005E450();
    func_8001D230();
    func_80037C5C();
    WORK->x3EE = 0xFF;
    func_8002EB14(0, 0x14, 1);
    WORK->x4C6 = 0;
    func_80034C48(2, 4, WORK->x108C);
    func_80034C48(2, 0xE, WORK->x108D);
    func_80034C48(2, 0x1C, WORK->x108E);
    func_80023D8C();
    func_80034C48(0xB, func_8004B778(WORK->xC2D, WORK->xC2C) + 0x75, 1);
    func_80034C48(0xB, func_80035A88(WORK->xC2D, WORK->xC2C), 1);
    if ((unsigned int)(*(unsigned short *)&WORK->xC2C - 4) < 2) {
        func_80034C48(0xB, 0, 1);
        func_80034C48(0xB, 4, 0);
        func_80034C48(0xB, 5, 0);
    }
}
