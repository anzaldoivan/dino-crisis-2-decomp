#include "common.h"
#include "dc2.h"

/* func_80037824 */
typedef struct {
    short a;
    short b;
} Pair;

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;

void func_80037824(int a0, int a1)
{
    D_800893D0.a = a0;
    if (D_800ABA96 == 0) {
        a1 += 0x100;
    }
    D_800893D0.b = a1;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003784C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800379C0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037AA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037B7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037BBC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037C1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037C5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037C9C);

void func_80037E18(Unit *self)
{
    unsigned int cnt[2];
    Unit *u;

    cnt[1] = 0;
    cnt[0] = 0;
    for (u = WORK->units; u < WORK->units + D_800AF11C; u++) {
        if (u->flags & 1) {
            cnt[u->side]++;
        }
    }
    self->side = cnt[0] >= cnt[1];
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037ED8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037FD4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038354);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038568);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038780);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003889C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038930);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038FB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800391E8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039430);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039654);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039978);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039A00);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039B7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039C20);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80039C54);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003A160);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003A410);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003A418);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003A954);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003AC70);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003B08C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003B494);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003B58C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003B778);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003B9F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003C498);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003C688);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003C8D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003C9D0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003CADC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003CB5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003CFC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003D1E0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003D22C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003D29C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003D2D4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003D58C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003DD34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003DD90);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003DE8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003DF80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E020);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E144);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E164);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E1B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E230);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E238);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E378);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E488);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E4E0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E63C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E644);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E6E4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E740);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E77C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E79C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E7B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E830);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003E878);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003EA1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003EA64);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003EAAC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003EB18);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003EB20);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003F154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003F8CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003FDA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003FE00);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003FFE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800400F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800402EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80040380);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80040444);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800404D4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004084C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80040AB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80040B94);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80040DB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80040F7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004104C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800412C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041338);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041778);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041810);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800418DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041994);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041A68);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041AE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041BB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041C30);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80041DC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80042108);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004262C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80042758);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004283C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800428D0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800428D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80042AE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80042D3C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80042E84);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043138);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043284);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800434DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800436C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043714);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800437F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043884);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043958);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043A2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043A7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043B24);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043B60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043BF8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043C8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043CC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043D94);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043E1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043E74);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80043F54);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044078);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800441A0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044220);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800442D4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004445C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800444F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044834);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044A88);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044AB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044B80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80044DC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004526C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004538C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045460);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800454BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045638);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004579C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045824);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004587C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800458F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045A20);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045B8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045C18);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80045D68);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046338);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046470);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004653C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800469D0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800469D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046BBC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046C58);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046D10);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046E2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046F6C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80046FC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047168);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800471CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800472F4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800473A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047450);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800474B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800474DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047584);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004758C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047714);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047814);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800478B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047958);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047960);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800479FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047AA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047AD4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047B60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047B8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047D5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047E0C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047E78);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80047FD0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048048);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800480F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800481A8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048248);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048274);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048338);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004840C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800484F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800487B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048AE0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048F60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80048FF0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800490AC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049134);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004916C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004920C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004924C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004927C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004948C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004969C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049900);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049C60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049D10);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049DAC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049E44);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049EA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049EEC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049F24);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049F7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80049FD0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A050);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A0F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A144);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A194);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A240);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A2C4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A2F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A32C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A378);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A464);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A4D4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A4E8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A4FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A554);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A610);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A66C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A790);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A84C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A8C0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A970);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A9A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004A9E4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004AA70);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004AABC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004AB14);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004ABB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004AC08);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004ADD0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004AFC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B298);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B35C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B428);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B448);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B468);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B488);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B4A8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B4C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B508);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B778);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B7B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004B8E8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004BA5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004BBF4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004BC58);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004BFC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004C0B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004C6DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004C960);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004CE64);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004D028);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004D274);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004D51C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004D964);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004DB98);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004DDC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004DFF4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004EEE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004EF50);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004F450);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004F710);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004F9A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004FE9C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8004FF38);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050030);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800501C4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050398);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050454);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050588);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800506BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050934);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800509C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050A40);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050A90);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050C90);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050D80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80050F50);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051054);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800513C0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800514C4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800517B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005189C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051998);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051B3C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051BAC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051C38);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051C80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051CD4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051D18);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051D58);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051DC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051E88);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80051F8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80052364);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800523EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005247C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005254C);

void func_80052634(void)
{
    WORK->x228++;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80052654);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005265C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800526B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80052AB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80052B34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80052C58);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005319C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053300);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053648);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800537C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053920);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053B1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053C9C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053DB0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80053EAC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800544FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80054A68);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80054B60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80054C58);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80055518);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80055724);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80055798);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800558F4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80055D5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80055F40);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005615C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005640C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056530);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056724);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056A18);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056B90);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056C80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056DFC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80056F00);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80057178);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80057A8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80057F48);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80058814);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80059150);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800591B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80059250);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80059294);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005978C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800597C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800598E0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80059CB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80059FB0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005A07C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005A144);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005A34C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005A37C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005A400);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005A580);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005AD28);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005AE5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005AF1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005B154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005B6B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005B928);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005BFC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005C608);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005CB10);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005CBE8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005CC04);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005CE28);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D0CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D4BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D72C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D930);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D940);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D97C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005D9C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005DA04);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005DA50);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005DCE0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005DEF0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E160);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E1DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E450);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E518);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E5BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E6A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E7D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E844);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E854);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E8F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005E940);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EA34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EAB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EB34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EC80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005ECB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005ECCC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005ED34);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EDA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EE58);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EE98);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EEF4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EF48);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EF8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EFE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005EFFC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F024);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F088);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F0B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F1EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F354);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F3B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F410);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F45C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F4BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F540);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F714);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F7B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F824);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005F878);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005FB44);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005FCD0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005FEE0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005FFB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8005FFC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006004C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060460);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800604EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800604F4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060538);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006059C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060738);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060848);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060968);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060A24);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060A2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060E18);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060EAC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060EB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060EF0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060F2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060FBC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80060FE8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061054);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800610A0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800610EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006113C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061188);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800611EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800614B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800614F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006155C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800615EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061680);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061798);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061948);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061A84);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061BAC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061CFC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061D14);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80061FD4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800620FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062210);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062324);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062474);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006266C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062794);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062820);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800629BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062AC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062E80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80062EF4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063040);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063084);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063158);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063194);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800631D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006321C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006325C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006335C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800633E8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063468);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006351C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800635E8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063610);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063654);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063700);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800637D0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800637E8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800638F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006393C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063988);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063A10);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063A8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063ADC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063C50);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063CC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063CFC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80063D78);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800640B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064238);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064314);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064434);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064460);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800644CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800644FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064578);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800645F4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064624);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064654);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800646B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006493C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064998);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800649A8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064A60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064AB4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064B88);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064BB0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064BB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064DCC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80064E0C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065004);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065038);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800652DC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065444);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006548C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800654F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065D0C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065EC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065EC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065EFC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065F24);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065F40);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80065F48);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006600C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066048);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006605C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066064);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006606C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066074);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800660B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066190);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800661C0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800661FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800662B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800663EC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006651C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800666C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800667B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066888);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066A8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066B80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066C20);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066C74);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066E20);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80066F78);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80067010);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80067B4C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80067F2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006837C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800689F4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80068CA0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800694A0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80069890);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800699A8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80069C00);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006AC8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006AF9C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006B2B8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006B308);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006B674);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006B938);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006BAC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006BBA8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006BCEC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006BD80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006C2D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006C454);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006C538);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006C5AC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006C78C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006CB1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006CD30);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006D7F4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006DABC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006DCE0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006DF00);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006E3AC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006E3B4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006E3F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8006E450);
