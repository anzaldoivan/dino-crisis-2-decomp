/* tools/probes/func_80037E18.c -- T4 probe (d) leaf with a stack frame: SLUS_012.79 game code (local counter
 * array on the stack, loop over a table reached through the scratchpad pointer). Our C; no game data. */
typedef struct {
    char pad0[0x50];
    int flags;
    char pad54[0xFD - 0x54];
    unsigned char side;
    char padfe[0x260 - 0xFE];
} Unit;

typedef struct {
    char pad0[0x4E0];
    Unit *units;
} Work;

#define WORK (*(Work **)0x1F800000)

extern int D_800AF11C;

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
