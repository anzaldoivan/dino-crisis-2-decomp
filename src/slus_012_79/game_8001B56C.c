#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned char x00[0x14];
    unsigned char x14[0x18];
    unsigned char x2C;
    unsigned char x2D[0x63];
} Ent0x8001b56c;
extern Ent0x8001b56c D_800ABF50[];
extern int D_800A79F0;
extern int func_8007B380(int);
extern int func_8007B594(Rect0x8001b56c *, int, int, int);
extern void *func_8007DF34(void *, int, int, int, int);
extern void *func_8007DFF4(void *, int, int, int, int);

void func_8001B56C(int a0, unsigned int a1)
{
    Rect0x8001b56c r;

    r.x0 = 0;
    r.x2 = 0;
    r.x4 = a0;
    r.x6 = 0x1F0;
    func_8007B594(&r, 0, 0, 0);
    func_8007B380(0);
    if (a1 > 0xF0) {
        func_8007DF34(D_800ABF50[0].x14, 0, 0, a0, a1);
        func_8007DF34(D_800ABF50[1].x14, 0, 0, a0, a1);
        func_8007DFF4(&D_800ABF50[0], 0, 0, a0, a1);
        func_8007DFF4(&D_800ABF50[1], 0, 0, a0, a1);
        D_800A79F0 = 0;
    } else {
        func_8007DF34(D_800ABF50[0].x14, 0, 0, a0, a1);
        func_8007DF34(D_800ABF50[1].x14, 0, 0x100, a0, a1);
        func_8007DFF4(&D_800ABF50[0], 0, 0x100, a0, a1);
        func_8007DFF4(&D_800ABF50[1], 0, 0, a0, a1);
        D_800A79F0 = 2;
    }
    D_800ABF50[1].x2C = 1;
    D_800ABF50[0].x2C = 1;
}
