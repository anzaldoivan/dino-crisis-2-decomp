#include "common.h"

extern void func_80034C48(int, int, int);
extern void func_80043D94(int);
extern int func_80075148(int, int, int, int);
extern void func_800782E4(void);
extern unsigned char D_800AF5C0[];

void func_8001D9E4(void)
{
    func_800782E4();
    func_80043D94(0);
    while (func_80075148(9, 0, 0, -1) == 0) {
    }
    D_800AF5C0[0x6A] = 0;
    func_80034C48(2, 0, 0);
}
