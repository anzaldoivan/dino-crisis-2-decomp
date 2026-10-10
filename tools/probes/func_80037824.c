/* tools/probes/func_80037824.c -- T4 probe (b) global data read/write: SLUS_012.79 game code; small globals
 * accessed absolute (lui/%lo), so -G0 vs -G4/-G8 splits. Our C; no game data. */
typedef struct {
    short a;
    short b;
} Pair;

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

void func_80037824(int a0, int a1)
{
    D_800893D0.a = a0;
    if (D_800ABA96 == 0) {
        a1 += 0x100;
    }
    D_800893D0.b = a1;
}
