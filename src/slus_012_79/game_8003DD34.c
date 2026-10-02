#include "common.h"
#include "dc2.h"
#include "../shared/func_800436C8.h"
#include "../shared/func_8002B3E0.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;



INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003DD34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003DD90);

unsigned short func_8003DE8C(short a, short b, short c, short d)
{
    if (c & 0x800) {
        c |= 0xF000;
    }
    if (d < (c < 0 ? -c : c)) {
        if (c & 0x800) {
            c = (c - a) | 0xF000;
            if (c < (short)-b) {
                c = -b;
            }
        } else {
            c = (c - a) & 0xFFF;
            if (b < c) {
                c = b;
            }
        }
    } else {
        c = (c - a) & 0xFFF;
        if (c & 0x800) {
            c |= 0xF000;
            if (c < (short)-b) {
                c = -b;
            }
        } else if (b < c) {
            c = b;
        }
    }
    return c;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003DF80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E020);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E144);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E164);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E1B4);

void func_8003E230(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E238);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E378);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E488);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E4E0);

void func_8003E63C(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E644);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E6E4);

#define SHARED_D0 D_800894C8
void func_8003E740(Obj8002B3E0_s8002B3E0 *obj)
{
#include "../shared/func_8002B3E0.inc.c"
}

#define SHARED_F0 func_800487B8
void func_8003E77C(void)
{
#include "../shared/func_8001EA54.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E79C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E7B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E830);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003E878);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003EA1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003EA64);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003EAAC);

void func_8003EB18(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003EB20);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003F154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003F8CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003FDA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003FE00);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8003FFE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800400F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800402EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040380);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040444);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800404D4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004084C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040AB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040B94);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040DB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80040F7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004104C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800412C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041338);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041778);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041810);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800418DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041994);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041A68);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041AE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041BB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041C30);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80041DC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80042108);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004262C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80042758);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004283C);

void func_800428D0(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800428D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80042AE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80042D3C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80042E84);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043138);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043284);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800434DC);

int func_800436C8(V3s_s800436C8 *a, V3s_s800436C8 *b)
{
#include "../shared/func_800436C8.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043714);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800437F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043884);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043958);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043A2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043A7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043B24);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043B60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043BF8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043C8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043CC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043D94);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043E1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043E74);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80043F54);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044078);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800441A0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044220);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800442D4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004445C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800444F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044834);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044A88);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044AB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044B80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80044DC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004526C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004538C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80045460);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_800454BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80045638);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_8004579C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8003DD34", func_80045824);


extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


