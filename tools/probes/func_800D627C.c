/* tools/probes/func_800D627C.c -- T2 (1.8) pin proof, family misc: overlay bin_ending game code (one call
 * with constants, a scratchpad work field and the second argument, six args). Our C; no game data. */
typedef struct {
    char pad0[0xDA8];
    int xda8;
} Work;

#define WORK (*(Work **)0x1F800000)

extern void func_800D5E50(int a, int b, int c, int d, int e, int f);

void func_800D627C(int x, int y)
{
    func_800D5E50(0xAA, 0x6D, WORK->xda8, 7, y, 1);
}
