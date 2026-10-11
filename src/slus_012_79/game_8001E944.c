#include "common.h"

typedef struct {
    unsigned char pad0[0x4C];
    unsigned int t4C;
    unsigned int t50;
    unsigned int t54;
    unsigned char pad58[0x14];
    unsigned char b6C;
    unsigned char b6D;
    unsigned char pad6E[0x1A];
    int n88;
    unsigned char pad8C[4];
    unsigned short n90;
} Obj0x8001e944;
extern Obj0x8001e944 D_800AF5C0;
extern void func_80043D94(int);
extern int func_80075148(int, int, int, int);
extern void func_800782E4(void);
extern int func_800786B4(unsigned char *);
extern int func_800787B4(int);

void func_8001E944(unsigned char mode, unsigned char *buf)
{
    Obj0x8001e944 *o = &D_800AF5C0;

    func_800787B4(0);
    if (mode == 2) {
        o->b6D = 0;
        o->t4C = func_800786B4(buf + 5);
        if (o->t4C >= o->t50) {
            func_800782E4();
            func_80043D94(0);
            while (func_80075148(9, 0, 0, -1) == 0) {
            }
            o->b6C = 3;
        }
        if (o->t4C < o->t54) {
            func_800782E4();
            func_80043D94(0);
            while (func_80075148(9, 0, 0, -1) == 0) {
            }
            o->b6C = 3;
            o->n88++;
            o->n90++;
        }
        o->t54 = o->t4C;
    }
}
