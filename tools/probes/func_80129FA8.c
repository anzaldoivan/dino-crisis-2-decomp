/* tools/probes/func_80129FA8.c -- T2 (1.8) pin proof, family KOF: overlay bin_kof_p60p game code (state byte
 * test, then two calls with the object). Our C; no game data. */
typedef struct {
    char pad0[0x56];
    unsigned char x56;
} Obj;

extern void func_80044834(Obj *p, int n);
extern void func_8001C680(Obj *p, void *q);

void func_80129FA8(Obj *p)
{
    if (p->x56 == 1) {
        func_80044834(p, 0x44);
        func_8001C680(p, (void *)0x801774C8);
    }
}
