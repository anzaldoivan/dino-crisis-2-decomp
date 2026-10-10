/* tools/probes/func_800D5C40.c -- T4 probe (c) loop + call: overlay psx_bin_st6 game code (counted loop over
 * a global table, then one call). Our C; no game data. */
typedef struct {
    int a;
    int b;
    int c;
} Entry;

typedef struct {
    char pad0[0x98];
    int x98;
    short pad9c;
    unsigned short x9e[8];
} Obj;

extern Entry D_800D6A70[];
extern void func_80047814(Obj *p);

void func_800D5C40(Obj *p)
{
    int i;

    D_800D6A70[0].c = p->x98;
    for (i = 0; i < 8; i++) {
        D_800D6A70[i + 1].c = p->x9e[i] + D_800D6A70[i].c;
    }
    func_80047814(p);
}
