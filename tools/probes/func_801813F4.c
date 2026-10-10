/* tools/probes/func_801813F4.c -- T2 (1.8) pin proof, family WEP_S: overlay bin_wep_s03 game code (store a
 * call result and three halfwords into a record reached through the scratchpad work pointer). Our C; no game
 * data. */
typedef struct {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x54 - 0x46];
    int x54;
} Rec;

typedef struct {
    char pad0[0x4DC];
    Rec *x4dc;
    char pad4e0[0xC2B - 0x4E0];
    unsigned char xc2b;
} Work;

typedef struct {
    char pad0[0x64];
    int x64;
} Obj;

#define WORK (*(Work **)0x1F800000)

extern int func_800474B4(int a, int n);

void func_801813F4(Obj *p)
{
    Rec *r = WORK->x4dc;

    r->x54 = func_800474B4(p->x64, 12);
    if (WORK->xc2b == 0) {
        r->x40 = -0x82;
    } else {
        r->x40 = -0xB4;
    }
    r->x42 = -0xA0;
    r->x44 = 0;
}
