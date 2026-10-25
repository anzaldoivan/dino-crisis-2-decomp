#include "common.h"

#include "dc2.h"
#define addPrim0x8002a77c(ot, p) \
    (((Tag0x8004c960 *)(p))->addr = ((Tag0x8004c960 *)(ot))->addr, \
     ((Tag0x8004c960 *)(ot))->addr = (unsigned int)(p))
extern unsigned short D_800AF140[];
extern int D_80088DC4;
extern int D_80088DC0;
extern short D_80088DBC;
extern unsigned char D_800ABA96;
extern unsigned int *D_800ABA98;
extern char D_800183EC[];
void func_80034F08(int x, int y, int z, char *fmt, ...);
extern int func_80078C44(int a);
extern void func_8007E134();

void func_8002A77C(void)
{
    Box0x8002243c r;
    int i;
    int v;

    if (D_800AF140[1] & 0x8000) {
        D_80088DC4++;
    }
    if (D_800AF140[1] & 0x2000) {
        D_80088DC4--;
    }
    if (D_800AF140[0] & 0x1000) {
        D_80088DC0++;
    }
    if (D_800AF140[0] & 0x4000) {
        D_80088DC0--;
    }
    if (D_800AF140[0] & 0x8) {
        D_80088DBC = 0;
    }
    func_80034F08(100, 100, 0x400, D_800183EC, D_80088DC4, D_80088DC0, D_80088DBC);
    D_80088DBC = (D_80088DBC + D_80088DC0) & 0xFFF;
    for (i = 0; i < 240; i++) {
        v = func_80078C44(D_80088DBC + i * D_80088DC0) >> D_80088DC4;
        if (v != 0) {
            if (v > 0) {
                r.x = 0;
                r.y = 240;
                r.w = 320 - v;
                r.h = 1;
                func_8007E134(PKT, &r, v, D_800ABA96 ? i + 0x100 : i);
                addPrim0x8002a77c(&D_800ABA98[28], PKT);
                PKT += 6;
            } else {
                r.x = -v;
                r.y = 240;
                r.w = v + 320;
                r.h = 1;
                func_8007E134(PKT, &r, 0, D_800ABA96 ? i + 0x100 : i);
                addPrim0x8002a77c(&D_800ABA98[28], PKT);
                PKT += 6;
            }
            r.x = 0;
            r.y = D_800ABA96 ? i + 0x100 : i;
            r.w = 320;
            r.h = 1;
            func_8007E134(PKT, &r, 0, 240);
            addPrim0x8002a77c(&D_800ABA98[28], PKT);
            PKT += 6;
        }
    }
}
