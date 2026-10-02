/* tools/probes/func_8017FA84.c -- T2 (1.8) pin proof, family WEP: overlay bin_wep0c game code (one-shot
 * state step: call once, then advance the state byte). Our C; no game data. */
typedef struct {
    char pad0[0x56];
    unsigned char x56;
} Obj;

extern void func_80044AB4(int n);

void func_8017FA84(Obj *p)
{
    if (p->x56 == 0) {
        func_80044AB4(30);
        p->x56++;
    }
}
