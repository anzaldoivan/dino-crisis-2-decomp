#include "dc2.h"

extern int func_80034CD8(int, int);
extern int func_8004ABB4();
extern unsigned short D_80019688[];
typedef struct {
    char pad0[0x58];
    unsigned char x58;
    char pad59[0x10C - 0x59];
    unsigned short x10C;
    char pad10E[0x11D - 0x10E];
    signed char x11D;
} Obj0x8002083c;
typedef struct {
    char pad0[0xB4];
    int xB4;
    char padB8[0xC4 - 0xB8];
    short xC4;
    char padC6[0xCA - 0xC6];
    unsigned char xCA;
    char padCB[1];
    unsigned char xCC;
    unsigned char xCD;
} Sub0x8002083c;
typedef struct {
    char pad0[0x22C];
    Sub0x8002083c x22C;
    char pad[0x7B4 - 0x22C - sizeof(Sub0x8002083c)];
    int x7B4;
    char pad7B8[0x7C0 - 0x7B8];
    unsigned short x7C0;
    unsigned short x7C2;
    unsigned short x7C4;
    char pad7C6[0xC2A - 0x7C6];
    signed char xC2A;
    char padC2B[1];
    unsigned short xC2C;
    char padC2E[0xD90 - 0xC2E];
    int xD90;
    char padD94[0xDA8 - 0xD94];
    int xDA8;
} Wk0x8002083c;
#define W0x8002083c ((Wk0x8002083c *)WORK)

void func_8002083C(Obj0x8002083c *obj)
{
    unsigned int val;
    Sub0x8002083c *sub;

    val = D_80019688[obj->x58];
    sub = &W0x8002083c->x22C;
    if (obj->x58 == 8 && W0x8002083c->xC2C == 0x808) {
        val = 1000;
    }
    if (obj->x58 == 2) {
        if (W0x8002083c->xC2C == 0x407) {
            val = 50;
        }
        if (obj->x10C == 5) {
            val = 7000;
        }
    }
    if (func_80034CD8(9, 0x36)) {
        val <<= 1;
    }
    if (W0x8002083c->xC2A == 0) {
        val <<= 1;
    }
    if (obj->x11D > 100) {
        obj->x11D = 100;
    } else if (obj->x11D < -100) {
        obj->x11D = -100;
    }
    val = val * (obj->x11D + 100) / 100;
    func_8004ABB4();
    if (sub->xCD == 0) {
        sub->xC4 = 0;
        sub->xB4 = 0;
    }
    if (sub->xCC == 0) {
        W0x8002083c->x7C0++;
        W0x8002083c->x7C2 = obj->x58;
    }
    if (sub->xCD != 0) {
        if (sub->xC4 >= 10) {
            val += val * 2;
        } else {
            val += val * sub->xC4 / 5;
        }
    }
    sub->xC4++;
    if (W0x8002083c->x7C4 < sub->xC4) {
        W0x8002083c->x7C4 = sub->xC4;
    }
    sub->xCA = 1;
    W0x8002083c->xD90 += val;
    sub->xB4 += val;
    W0x8002083c->x7B4 += val;
    W0x8002083c->xDA8 += val;
    if (W0x8002083c->xD90 > 9999999) {
        W0x8002083c->xD90 = 9999999;
    }
    sub->xCD = 1;
}
