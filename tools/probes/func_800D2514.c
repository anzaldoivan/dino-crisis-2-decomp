/* tools/probes/func_800D2514.c -- T2 (1.8) pin proof, family E: overlay bin_e60 game code (test call on a
 * member, then a conditional call with the object). Our C; no game data. */
typedef struct {
    char pad0[0xD8];
    char xd8[0x1C0 - 0xD8];
    char x1c0[4];
} Obj;

extern int func_80034D60(void *p, int n);
extern void func_80049F24(Obj *p, int n, void *q);

void func_800D2514(Obj *p)
{
    if (func_80034D60(p->x1c0, 3)) {
        func_80049F24(p, 13, p->xd8);
    }
}
