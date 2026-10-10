/* tools/probes/func_800DB960.c -- T4 probe (e) tie-breaker: overlay psx_bin_st9 game code, two signed divides
 * of scaled halfwords through a global pointer, then a three-way compare. Our C; no game data. */
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
