#include "common.h"

#include "dc2.h"
typedef Rect0x8006c2d8 Rect0x8001bc7c;
typedef struct {
    char disp[0x14];
    char draw[0x5C];
    unsigned int ot[8];
} Db0x8001bc7c;
extern int func_8007B380(int);
extern void func_8007B504(Rect0x8001bc7c *, int, int, int);
extern int func_8007B86C(void *, int);
extern void *func_8007DF34(void *, int, int, int, int);
extern void *func_8007DFF4(void *, int, int, int, int);
extern Db0x8001bc7c D_800ABF50[2];
extern unsigned int D_800AC070[2][0x400];
extern unsigned int D_800ABA78[2][3];
extern unsigned int D_800AF598[2][4];
extern unsigned char D_800ABA96;
extern Db0x8001bc7c *D_800ABA98;

void func_8001BC7C(void)
{
    Rect0x8001bc7c rect;
    unsigned int i;

    rect.x = 0;
    rect.y = 0;
    rect.w = 0x400;
    rect.h = 0x200;
    func_8007B504(&rect, 0, 0, 0);
    func_8007B380(0);
    func_8007DF34(D_800ABF50[0].draw, 0, 0, 320, 240);
    func_8007DFF4(D_800ABF50[0].disp, 0, 256, 320, 240);
    func_8007DF34(D_800ABF50[1].draw, 0, 256, 320, 240);
    func_8007DFF4(D_800ABF50[1].disp, 0, 0, 320, 240);
    for (i = 0; i < 2; i++) {
        func_8007B86C(D_800ABF50[i].ot, 8);
        func_8007B86C(D_800AC070[i], 0x400);
        func_8007B86C(D_800ABA78[i], 3);
        func_8007B86C(D_800AF598[i], 4);
    }
    D_800ABA96 = 0;
    D_800ABA98 = D_800ABF50;
    PKT = D_801A1500;
}
