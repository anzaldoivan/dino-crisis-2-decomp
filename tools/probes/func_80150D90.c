/* tools/probes/func_80150D90.c -- T2 (1.8) pin proof, family RES: overlay bin_res01 game code (load a colour
 * word into the GTE, then two calls on an embedded member with the same second argument). Our C; no game
 * data. gte_ldrgb follows the PsyQ inline_c.h macro of that name. */
#define gte_ldrgb(r0) __asm__ volatile("lwc2 $6, 0( %0 )" : : "r"(r0))

typedef struct {
    char pad0[0x1C];
    char x1c[4];
} Obj;

extern void func_801507F0(void *p, int a);
extern void func_80150AF8(void *p, int a);

void func_80150D90(Obj *p, int a)
{
    void *q = p->x1c;
    int rgb = 0x80;

    gte_ldrgb(&rgb);
    func_801507F0(q, a);
    func_80150AF8(q, a);
}
