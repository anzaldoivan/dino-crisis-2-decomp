#include "common.h"

extern int D_800AF5BC;
extern unsigned char D_800AF5C0[];
extern void func_80018020(void *, int);
extern int func_80074EF8(void);
extern void func_80078734(int);
extern void func_80077D8C(int);
extern int func_80075148(int, int, int, int);

void func_8001D964(void)
{
    unsigned char buf[8];

    buf[0] = 0x80;
    func_80018020(D_800AF5C0, 0x2A);
    while (func_80074EF8() == 0) {
    }
    func_80078734(0);
    func_80077D8C(1);
    while (func_80075148(0xE, (int)buf, 0, -1) == 0) {
    }
    D_800AF5BC = 0;
}
