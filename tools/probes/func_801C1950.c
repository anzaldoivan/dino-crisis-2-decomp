/* tools/probes/func_801C1950.c -- T2 (1.8) pin proof, family R2: overlay bin_subscr6 game code (reset a run
 * of byte and halfword fields, then one call with the object). Our C; no game data. */
typedef struct {
    char pad0[2];
    char x2;
    char pad3[0x4E - 3];
    char x4e;
    char x4f;
    char pad50[0x80 - 0x50];
    short x80;
    short x82;
    char pad84[0x8E - 0x84];
    short x8e;
    short x90;
    short x92;
    short x94;
    short x96;
    short x98;
    short x9a;
    short x9c;
} Obj;

extern void func_801C61CC(Obj *p);

void func_801C1950(Obj *p)
{
    p->x2 = 1;
    p->x4f = 0;
    p->x4e = 0;
    p->x92 = 0;
    p->x94 = 0;
    p->x96 = 0;
    p->x98 = 0;
    p->x80 = 0;
    p->x82 = 0;
    p->x9a = 0;
    p->x9c = 0;
    p->x8e = 30;
    p->x90 = 0;
    func_801C61CC(p);
}
