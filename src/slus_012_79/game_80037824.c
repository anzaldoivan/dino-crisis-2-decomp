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


typedef struct {
    unsigned char b0;
    unsigned char pad[0x27];
} Elem28;
extern Elem28 D_800B4A68[];


