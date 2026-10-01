/* tools/probes/func_80052634.c -- T5 standalone draft (SLUS_012.79): scratchpad work counter ++.
 * Self-contained as drafted from split asm; T5.c2's self-decl rung swaps the local Work/WORK for dc2.h. Our C; no game data. */
typedef struct {
    char pad0[0x228];
    int x228;
} Work;

#define WORK (*(Work **)0x1F800000)

void func_80052634(void)
{
    WORK->x228++;
}
