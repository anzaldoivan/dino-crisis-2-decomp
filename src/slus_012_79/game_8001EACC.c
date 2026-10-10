#include "common.h"

typedef struct {
    char pad0[0x24];
    char buf24[0x28];
    int f4C;
    char pad50[0x16];
    unsigned short f66;
    char pad68[2];
    unsigned char f6A;
    char pad6B;
    unsigned char f6C;
    unsigned char f6D;
    char pad6E[0x1A];
    int f88;
    unsigned short f8C;
} Obj0x8001eacc;
extern void func_8001E39C(void);
extern int func_800753BC();
int func_800782A4(void);
extern void func_800782E4(void);
extern void func_800785A4();
extern void func_8001F990();

void func_8001EACC(Obj0x8001eacc *p)
{
    if (!(func_800782A4() & 2)) {
        goto call;
    }
    {
        switch (p->f6C) {
        case 0:
            func_800785A4(p->f4C, p->buf24);
            p->f6D = 0;
            func_800782E4();
            do {
            } while (!func_800753BC(0xA0, p->buf24, 0x15, func_8001F990, -1));
            p->f6C++;
            break;
        case 1:
            if (++p->f6D >= 0x3D) {
                p->f6C = 0;
                p->f88++;
                p->f8C++;
                if (++p->f66 >= 3) {
                call:
                    func_8001E39C();
                }
            }
            break;
        case 2:
            p->f6A = 0;
            break;
        }
    }
}
