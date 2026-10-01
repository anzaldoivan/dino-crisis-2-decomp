#include "common.h"

/* func_800DB960 */
typedef struct {
    char pad0[0x368];
    short a;
    short b;
    char pad36c[0x5C8 - 0x36C];
    short c;
    short d;
} Stage;

extern Stage *D_800AE560;

int func_800DB960(void)
{
    Stage *s = D_800AE560;
    int x = s->a * 6000 / s->b;
    int y = s->c * 6000 / s->d;
    int r = y >= x;

    if (x == y) {
        r = -1;
    }
    return r;
}

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DB9F4);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DBA30);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DBA74);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DBB88);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DBB90);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DBBB0);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DC094);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DC21C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DC55C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DC8E8);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DC96C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DCACC);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DCBB8);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DCDBC);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DCEDC);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DCF48);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DCFAC);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD218);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD258);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD298);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD2D4);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD45C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD6A0);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD808);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD810);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DD964);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDC94);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDD90);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDE24);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDEA8);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDEE0);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDF2C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DDFD0);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE040);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE048);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE0C4);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE0D8);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE164);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE21C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE304);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE378);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE424);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE42C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE55C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE598);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE660);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE76C);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE7A8);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE7E4);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE7EC);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE874);

INCLUDE_ASM("asm/psx_bin_st9/nonmatchings/game_800DB960", func_800DE894);
