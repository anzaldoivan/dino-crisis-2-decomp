#include "common.h"

typedef struct {
    unsigned char pad0[0x6C];
    unsigned char b6C;
    unsigned char b6D;
    unsigned char pad6E[0x1A];
    int n88;
    unsigned char pad8C[4];
    unsigned short n90;
} Obj0x8001e898;
extern Obj0x8001e898 D_800AF5C0;
extern void func_80043D94(int);
extern int func_80075148(int, int, int, int);
extern void func_800782E4(void);
extern int func_800787B4(int);

void func_8001E898(unsigned char mode)
{
    Obj0x8001e898 *o = &D_800AF5C0;

    func_800787B4(0);
    if (mode == 2) {
        o->b6D = 0;
        func_80043D94(1);
    } else if (mode == 5) {
        func_800782E4();
        func_80043D94(0);
        while (func_80075148(9, 0, 0, -1) == 0) {
        }
        o->b6C = 3;
        o->n88++;
        o->n90++;
    }
}
