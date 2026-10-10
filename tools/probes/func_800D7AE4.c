/* tools/probes/func_800D7AE4.c -- T4 probe (a') signed int division: overlay psx_bin_st2 game code, one
 * divide by a halfword field, plus a function-address store. Our C; no game data. */
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
