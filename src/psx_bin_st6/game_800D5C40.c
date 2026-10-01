#include "common.h"
#include "dc2.h"

/* func_800D5C40: Entry, Obj in dc2.h */
extern Entry D_800D6A70[];
extern void func_80047814(Obj *p);

void func_800D5C40(Obj *p)
{
    int i;

    D_800D6A70[0].c = p->x98;
    for (i = 0; i < 8; i++) {
        D_800D6A70[i + 1].c = p->x9e[i] + D_800D6A70[i].c;
    }
    func_80047814(p);
}

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5CA0);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5CDC);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5CF0);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5D48);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5EC4);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5F00);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5F0C);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5F14);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D5FDC);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D60AC);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D61A4);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D6478);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D64CC);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D64D4);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D6510);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D662C);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D6708);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D6744);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D678C);

INCLUDE_ASM("asm/psx_bin_st6/nonmatchings/game_800D5C40", func_800D67A0);
