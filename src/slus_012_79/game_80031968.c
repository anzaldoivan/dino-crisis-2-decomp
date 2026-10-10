#include "dc2.h"

#define SPAD_8 (*SCRATCH(short, 0x8))
#define SPAD_A (*SCRATCH(short, 0xA))
extern signed char D_800B2488;
extern signed char D_800B2489;
extern signed char D_800B248A;
extern signed char D_800B248B;
extern short D_80089214;
extern unsigned short D_800ABA90;
extern short D_800ABA68;
extern void func_8001BE08();
extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern void func_80031084();
extern void func_800310B8();
extern void func_800317FC();
extern void func_80034C48(int, int, int);
extern void func_80044834(void *, int);
extern void func_8004526C();
extern void func_8004538C();
extern void func_8005AD28();
extern void func_8002F398();
extern void func_800315A4();

void func_80031968(void)
{
    int c0;
    int c1;
    unsigned short pad;

    func_80034C48(2, 0x22, 1);
    func_80034C48(2, 1, 0);
    func_80034C48(2, 0xD, 0);
    func_80034C48(2, 4, 1);
    func_8001BE08();
    D_800B2488 = 0;
    D_800B2489 = 0;
    D_800B248A = 0;
    D_800B248B = 0;
    D_80089214 = 0;
    if (*(unsigned short *)&WORK->xC2C == 0x80A || *(unsigned short *)&WORK->xC2C == 1) {
        D_800B2489 = 1;
    }
    func_8004526C();
    func_80031084();
    for (;;) {
        if (*(unsigned short *)&WORK->xC2C != 0x80A && *(unsigned short *)&WORK->xC2C != 1) {
            c0 = 0xC0C0;
            c1 = 0xC0C0;
            if (D_800B2489 == 0) {
                c1 = 0x303030;
            } else {
                c0 = 0x303030;
            }
            func_800317FC(D_80185D50, 0x6A, 0x50, 9, 0x7DCB, 0x1537D1);
            func_800317FC(D_80185D58, 0x78, 0x6C, 9, 0x7DCB, c0);
            func_800317FC(D_80185D88, 0x8A, 0x7A, 9, 0x7DCB, c1);
        } else {
            func_800317FC(D_80185D50, 0x6A, 0x70, 9, 0x7DCB, 0x1537D1);
        }
        func_800310B8();
        func_8005AD28();
        if (++D_800B248A >= 11) {
            D_800B248A = 11;
        }
        if (++D_80089214 >= 3) {
            D_80089214 = 2;
        }
        if (D_800B248A >= 3) {
            pad = D_800ABA90;
            if (pad & 0x5000) {
                if (*(unsigned short *)&WORK->xC2C == 0x80A || *(unsigned short *)&WORK->xC2C == 1) {
                    D_800B2489 = 1;
                } else {
                    D_800B248A = 0;
                    func_80044834(0, 0);
                    D_800B2489 ^= 1;
                }
            } else if (pad & 0x840) {
                func_80034C48(2, 4, WORK->x108C);
                func_80044834(0, 1);
                func_8004538C();
                func_80034C48(2, 0x10, 0);
                func_80034C48(2, 0x22, 0);
                SPAD_8 = 0;
                SPAD_A = 0;
                D_800ABA68 = 0;
                D_800ABA90 = 0;
                func_8001CED4((int)func_8002F398);
            } else if (pad & 0xA0) {
                func_80044834(0, 2);
                if (D_800B2489 == 0) {
                    WORK->x6E8 = 1;
                    D_800B2488 = 0;
                    func_8001CED4((int)func_800315A4);
                } else {
                    func_80034C48(2, 4, WORK->x108C);
                    func_8004538C();
                    func_80034C48(2, 0x10, 0);
                    func_80034C48(2, 0x22, 0);
                    SPAD_8 = 0;
                    SPAD_A = 0;
                    D_800ABA68 = 0;
                    D_800ABA90 = 0;
                    func_8001CED4((int)func_8002F398);
                }
            }
        }
        func_8001CDE4(1);
    }
}
