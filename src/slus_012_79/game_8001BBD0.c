#include "common.h"

extern unsigned char D_800ABAA0[];
extern unsigned char D_800A79BA[];
extern short D_800ABA94;
extern void func_80071694(void);
extern void func_8007B004(int);
extern void func_8007B178(int);
extern void func_80078D8C(void);
extern void func_8001D900(void);
extern void func_80082C04(void *, void *);
extern void func_80082774(int, void *, int);
extern void func_80082AF4(void);
extern void func_800437F8(void);
extern void func_80043958(void);
extern void func_800813B4(int);
extern void func_8001D964(void);
extern void func_80043A7C(void);

void func_8001BBD0(void)
{
    func_80071694();
    func_8007B004(0);
    func_8007B178(0);
    func_80078D8C();
    func_8001D900();
    D_800A79BA[1] = 0;
    D_800A79BA[0] = 0;
    func_80082C04(D_800ABAA0, D_800ABAA0 + 0x24);
    func_80082774(0, D_800A79BA, 2);
    func_80082AF4();
    func_800437F8();
    func_80043958();
    func_800813B4(0);
    func_8001D964();
    func_80043A7C();
    D_800ABA94 = 0xD37;
}
