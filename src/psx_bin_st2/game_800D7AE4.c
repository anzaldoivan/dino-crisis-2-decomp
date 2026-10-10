#include "common.h"
#include "../shared/func_80023E04.h"

/* func_800D7AE4 */
typedef struct {
    char pad0[3];
    unsigned char count;
    void (*fn)(void);
    char pad8[4];
    short span;
    short spanCopy;
    unsigned char b10;
    unsigned char b11;
    unsigned char b12;
    unsigned char step;
} Anim;

extern void func_800D7974(void);

void func_800D7AE4(Anim *p)
{
    p->b10 = 0;
    p->b11 = 0;
    p->b12 = 0;
    p->count++;
    p->spanCopy = p->span;
    p->step = 64 / p->span;
    p->fn = func_800D7974;
}

INCLUDE_ASM("asm/psx_bin_st2/nonmatchings/game_800D7AE4", func_800D7B38);

#define SHARED_D0 D_800D8084
void func_800D7BD4(Obj80023E04_s80023E04 *obj)
{
#include "../shared/func_80023E04.inc.c"
}
