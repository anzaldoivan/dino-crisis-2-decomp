#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x2E];
    unsigned char x2E;
    char pad2f[0x58 - 0x2F];
    short x58;
    short x5A;
    char pad5c[0x60 - 0x5C];
    short x60;
    short x62;
} Cam0x80039430;
typedef struct {
    char pad0[0x50];
    int x50;
    unsigned char x54;
    unsigned char x55;
    unsigned char x56;
    unsigned char x57;
    char pad58[0x92 - 0x58];
    unsigned short x92;
    char pad94[0xF0 - 0x94];
    short xF0;
    short xF2;
    short xF4;
    char padf6[0x108 - 0xF6];
    short x108;
    char pad10a[0x11B - 0x10A];
    unsigned char x11B;
    char pad11c[0x174 - 0x11C];
    short x174;
    unsigned short x176;
    short x178;
    char pad17a[0x1DC - 0x17A];
    int x1DC;
} Obj0x80039430;
extern unsigned char D_80018714[];
extern unsigned char D_80018718[];
extern unsigned char D_8001871C[];
extern Cam0x80039430 *D_800B25DC;
extern int func_8003C9D0(int);
extern void func_8003EA64();
extern void func_80044834();
extern void func_800484F0();
extern int func_800487B8();

void func_80039430(Obj0x80039430 *p)
{
    int st;
    int idx;

    switch (st = p->x56) {
    case 0:
        idx = p->x178;
        if (idx & 1) {
            p->xF0 = (signed char)(p->x55 ? D_80018718[idx] : D_80018714[idx]);
            p->xF4 = 0;
            p->xF2 = 0;
        } else {
            *(int *)&p->xF0 = 0;
            p->xF4 = (signed char)(p->x55 ? D_80018718[idx] : D_80018714[idx]);
        }
        if (p->x55) {
            idx = D_8001871C[idx];
            func_800484F0(p, idx, 0, 0, 4);
        } else {
            idx = D_8001871C[idx];
            func_800484F0(p, idx + 4, 0, 0x40, 4);
        }
        st = func_8003C9D0(p->x176);
        p->x11B = 0;
        p->x174 = 1;
        p->x56++;
        p->x108 -= st;
        D_800B25DC->x2E = 0;
        func_80044834(p, 3);
        D_800B25DC->x60 = 0;
        D_800B25DC->x62 = 0;
        D_800B25DC->x58 = 0;
        D_800B25DC->x5A = 0;
        if (p->x108 < 0) {
            *(int *)&p->x54 = 3;
        }
        break;
    case 1:
        if (func_800487B8(p)) {
            func_8003EA64(p);
            p->x1DC = 8;
            p->x174 = 0;
            *(int *)&p->x54 = 1;
            p->x50 &= ~0x10;
            p->x50 &= ~0x20;
            p->x50 &= ~0x800;
            p->x50 &= ~0x100;
        } else if (p->x92 >= 8) {
            *(int *)&p->xF0 = 0;
            p->xF4 = 0;
        }
        break;
    }
}
