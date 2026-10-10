#include "common.h"
#include "dc2.h"

typedef struct Bar0x80022ebc {
    char pad0[0xBE];
    short xBE;
} Bar0x80022ebc;
typedef struct Wk0x80022ebc {
    char pad0[0x22C];
    Bar0x80022ebc x22C;
    char pad2ec[0x4C8 - 0x22C - sizeof(Bar0x80022ebc)];
    short x4C8;
    short x4CA;
} Wk0x80022ebc;
extern unsigned int *D_800ABA98;
extern void func_80023990();
extern void func_80023AFC();
extern int func_8004A84C();

void func_80022EBC(int x, int y)
{
    Bar0x80022ebc *b;
    unsigned int col;
    unsigned int col2;
    short v;
    Box0x8002243c box0;
    Box0x8002243c box1;
    Box0x8002243c box2;

    b = &((Wk0x80022ebc *)WORK)->x22C;
    v = (((Wk0x80022ebc *)WORK)->x4C8 * 112) / ((Wk0x80022ebc *)WORK)->x4CA;
    if (v < 0) {
        v = 0;
    } else if (v > 0x70) {
        v = 0x70;
    }
    col = 0x808080;
    box0.x = x;
    box0.y = y;
    box0.w = 0x40;
    box0.h = 8;
    func_80023AFC(&box0, 0, 0xF8, 0x7E8F, col, 0);
    {
        unsigned int *ot;
        unsigned int *p;

        PKT[1] = 0xE100041A;
        PKT[2] = 0;
        ot = D_800ABA98;
        p = PKT;
        *p = ot[31] | 0x2000000;
        ot[31] = (unsigned int)p & 0xFFFFFF;
        PKT = PKT + 3;
    }
    func_80023990(D_801850A8, x + 0x40, y + 8, 12, 0x7C8F, col);
    func_80023990(D_80183620, x, y, 13, 0x7C8F, col);
    if (((Wk0x80022ebc *)WORK)->x4CA / 4 >= ((Wk0x80022ebc *)WORK)->x4C8) {
        col2 = func_8004A84C(0x80, 0x40, 3);
    } else if (((Wk0x80022ebc *)WORK)->x4CA / 2 >= ((Wk0x80022ebc *)WORK)->x4C8) {
        col2 = func_8004A84C(0x8080, 0x4040, 4);
    } else {
        col2 = func_8004A84C(0x8000, 0x4000, 5);
    }
    box1.x = x;
    box1.y = y;
    box1.w = v;
    box1.h = 16;
    func_80023AFC(&box1, 0x70, 0x50, 0x7C8F, col2, 0);
    b->xBE -= 6;
    if (b->xBE <= ((Wk0x80022ebc *)WORK)->x4C8) {
        b->xBE = ((Wk0x80022ebc *)WORK)->x4C8;
    } else {
        v = (b->xBE * 112) / ((Wk0x80022ebc *)WORK)->x4CA;
        if ((unsigned short)v <= 0x70) {
            box2.x = x;
            box2.y = y;
            box2.w = v;
            box2.h = 16;
            func_80023AFC(&box2, 0x70, 0x50, 0x7C8F, col2 >> 1, 0);
        }
    }
    {
        unsigned int *ot;
        unsigned int *p;

        PKT[1] = 0xE100040C;
        PKT[2] = 0;
        ot = D_800ABA98;
        p = PKT;
        *p = ot[31] | 0x2000000;
        ot[31] = (unsigned int)p & 0xFFFFFF;
        PKT = PKT + 3;
    }
}
