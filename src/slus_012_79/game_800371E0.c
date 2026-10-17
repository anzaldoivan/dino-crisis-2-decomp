#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x4BA];
    unsigned char x4BA;
} Wk0x800371e0;
#define WORK0x800371e0 (*SCRATCH(Wk0x800371e0 *, 0))
typedef struct Obj0x800371e0 {
    u16 x0;
    char pad2[2];
    int x4[2];
    int xC;
    int x10[2];
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
    char pad3c[4];
    short x40;
    short x42;
    short x44;
    short x46;
    char x48[6];
    unsigned short x4E;
} Obj0x800371e0;
typedef struct Ent0x800371e0 {
    int x0;
    char pad4[4];
    unsigned short x8;
    unsigned short xA;
    unsigned short xC;
    unsigned char xE;
    char padf[3];
    unsigned short x12;
} Ent0x800371e0;
typedef struct Pair0x800371e0 {
    int a;
    int b;
} Pair0x800371e0;
extern unsigned char D_800ABF50[];
extern Ent0x800371e0 D_800892F4[];
extern short D_800893D0[];
extern Pair0x800371e0 D_80085FB0[];
extern void func_80037694(Obj0x800371e0 *p);
extern int func_800379C0(int a);
extern void func_80037BBC(void *a);
extern int func_80071484(int arg0);
extern void func_80072114(int a, int b);
extern void func_80072224(int a, int b, int c, int d, int e);
extern int func_800722B4(int a);
extern void func_800785A4(int a, void *b);
extern void func_80085564(int a);
extern void func_800856BC(int a, int b);
extern void func_80085738(int a, int b);
extern void func_80085800(void (*f)());
extern int func_80085BF4(int a);
extern void func_80085C24(int a, int b);
extern void func_8003784C();

void func_800371E0(Obj0x800371e0 *p)
{
    Ent0x800371e0 *t;
    unsigned char mode;
    unsigned short w;
    unsigned short h;
    int r;
    unsigned int n;

    p->x0++;
    D_800ABF50[0x2C] = 0;
    D_800ABF50[0xBC] = 0;
    t = &D_800892F4[p->x4E];
    mode = t->xE;
    w = t->x8;
    h = t->xA;
    p->x30 = mode;
    p->x2C = t->x12;
    func_80037694(p);
    func_80085564(0);
    p->x40 = w;
    p->x34 = 1;
    p->x38 = 1;
    p->x42 = h;
    p->x46 = 0;
    p->x44 = w >> 4;
    p->x20 = 2;
    p->x24 = mode ? h * 48 : h * 32;
    p->x28 = mode ? h * 12 : h * 8;
    D_800893D0[2] = mode ? 24 : 16;
    D_800893D0[3] = h;
    p->x1C = p->x24 * p->x20 + t->xC;
    p->x18 = 0;
    p->xC = 0;
    p->x4[1] = 0x801B9500 - t->xC;
    p->x10[1] = p->x4[1] - p->x24 * p->x20;
    p->x4[0] = p->x4[1] - 0x18000;
    p->x10[0] = p->x10[1] - 0x18000;
    func_80072114(0x801C1500, 0x20);
    func_80085800(func_8003784C);
    D_800ABF50[0xA1] = mode;
    D_800ABF50[0x11] = mode;
    func_800785A4(D_80085FB0[t->x0].a, p->x48);
    func_80072224(p->x30, 1, -1, 0, 0);
    func_80037BBC(p->x48);
    r = 0;
    n = 0;

    for (;;) {
        while (WORK0x800371e0->x4BA != 2) {
            r = func_800379C0(p->x2C);
            if (r != 0) {
                break;
            }
            func_80071484(0);
            n++;
        }
        if (r != 0) {
            break;
        }
        if (n < 240) {
            WORK0x800371e0->x4BA = 1;
        }
        if (WORK0x800371e0->x4BA == 2) {
            return;
        }
    }

    func_80085BF4(0);
    func_80085C24(r, p->x4[p->xC]);
    func_800722B4(r);
    n = 0;
    func_800856BC(p->x4[p->xC], p->x30);
    func_80085738(p->x10[p->x18], p->x28 * p->x20);
    p->xC ^= 1;

    for (;;) {
        while (WORK0x800371e0->x4BA != 2) {
            r = func_800379C0(p->x2C);
            if (r != 0) {
                break;
            }
            func_80071484(0);
            n++;
        }
        if (r != 0) {
            break;
        }
        if (n < 240) {
            WORK0x800371e0->x4BA = 1;
        }
        if (WORK0x800371e0->x4BA == 2) {
            return;
        }
    }

    func_80085C24(r, p->x4[p->xC]);
    func_800722B4(r);
    WORK0x800371e0->x4BA = 1;
    func_80071484(0);
}
