#include "dc2.h"

typedef struct Unit0x8002243c {
    char pad0[0x108];
    short x108;
    short x10A;
    char pad10c[0x17E - 0x10C];
    unsigned short x17E;
} Unit0x8002243c;
typedef struct Bar0x8002243c {
    char pad0[0xBC];
    short xBC;
} Bar0x8002243c;
typedef struct Wk0x8002243c {
    char pad0[4];
    int x4;
    char pad8[0x22C - 0x8];
    Bar0x8002243c x22C;
    char pad2ea[0x4E0 - 0x22C - sizeof(Bar0x8002243c)];
    Unit0x8002243c *units;
} Wk0x8002243c;
extern unsigned int *D_800ABA98;
extern void func_80023990();
extern void func_80023AFC();
extern int func_8004A84C();

void func_8002243C(int x, int y)
{
    Wk0x8002243c *w;
    Unit0x8002243c *u;
    Bar0x8002243c *b;
    unsigned int col;
    short v;
    short v2;
    Box0x8002243c box;
    unsigned int *ot;
    unsigned int *p;

    w = (Wk0x8002243c *)WORK;
    u = w->units;
    b = &w->x22C;
    if (u->x10A / 4 >= u->x108 && (w->x4 & 0x10)) {
        func_80023990(D_80185020, x - 2, y - 2, 12, 0x7C8F, 0x80);
    }
    if (u->x17E) {
        col = func_8004A84C(0x80, 0x30, 3) | 0x2000000;
    } else {
        col = 0x808080;
    }
    func_80023990(D_801850A8, x + 0x40, y + 8, 12, 0x7C8F, col);
    func_80023990(D_80183620, x, y, 13, 0x7C8F, col);
    v = (u->x108 * 112) / u->x10A;
    if (v < 0) {
        v = 0;
    } else if (v > 0x70) {
        v = 0x70;
    }
    if (u->x10A / 4 >= u->x108 || u->x17E != 0) {
        col = func_8004A84C(0x80, 0x40, 3);
    } else if (u->x10A / 2 >= u->x108) {
        col = func_8004A84C(0x8080, 0x4040, 4);
    } else {
        col = func_8004A84C(0x8000, 0x4000, 5);
    }
    box.x = x;
    box.y = y;
    box.w = v;
    box.h = 16;
    func_80023AFC(&box, 0x70, 0x50, 0x7C8F, col, 0);
    b->xBC -= 6;
    if (b->xBC <= u->x108) {
        b->xBC = u->x108;
    } else {
        v2 = (b->xBC * 112) / u->x10A;
        if (v2 < 0) {
            v2 = 0;
        } else if (v2 > 0x70) {
            v2 = 0x70;
        }
        box.x = x;
        box.y = y;
        box.w = v2;
        box.h = 16;
        func_80023AFC(&box, 0x70, 0x50, 0x7C8F, col >> 1, 0);
    }
    PKT[1] = 0xE100040C;
    PKT[2] = 0;
    ot = D_800ABA98;
    p = PKT;
    *p = ot[31] | 0x2000000;
    ot[31] = (unsigned int)p & 0xFFFFFF;
    PKT = PKT + 3;
}
