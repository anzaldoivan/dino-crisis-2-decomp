#include "common.h"
#include "dc2.h"

extern u32 *D_800ABA98;
void func_80021F78(s16 *rect, s16 u, s32 v, s32 clut, s32 a4, s32 a5, s32 a6);

void func_8002204C(u16 x, u16 y, u32 value, s32 arg3)
{
    s16 rect[4];
    u32 div;
    u16 digit;
    u16 i;
    u32 *ot;
    u32 *p;

    div = 10;
    for (i = 0; i < 2; i++) {
        digit = value / div;
        rect[0] = x + (i << 4);
        rect[1] = y;
        rect[2] = 16;
        rect[3] = 16;
        value -= digit * div;
        div /= 10;
        func_80021F78(rect, (digit << 4) + 0x50, 0x30, 0x7DCC, arg3, 0, 1);
    }
    (*(u32 **)0x1F800004)[1] = 0xE1000408;
    (*(u32 **)0x1F800004)[2] = 0;
    ot = D_800ABA98;
    p = *(u32 **)0x1F800004;
    *p = ot[31] | 0x02000000;
    ot[31] = (u32)p & 0xFFFFFF;
    *(u32 *)0x1F800004 += 0xC;
}
