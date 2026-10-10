/* tools/probes/func_801C1F88.c -- T2 (1.8) pin proof, family MAP: overlay psx_data_map game code (call with
 * a signed byte field, then copy it to scratchpad work and derive two byte fields). Our C; no game data. */
typedef struct {
    char pad0[2];
    char x2;
    char pad3[0x60 - 3];
    signed char x60;
    char pad61;
    char x62;
} Obj;

typedef struct {
    char pad0[0x6E8];
    char x6e8;
} Work;

#define WORK (*(Work **)0x1F800000)

extern void func_8004262C(int n);

void func_801C1F88(Obj *p)
{
    func_8004262C(p->x60);
    WORK->x6e8 = p->x60;
    p->x2 = 3;
    p->x62 = p->x60 - 5;
}
