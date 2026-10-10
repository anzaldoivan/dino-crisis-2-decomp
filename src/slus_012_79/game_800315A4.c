#include "dc2.h"

typedef struct WorkView0x800315a4 {
    char pad0[0x318];
    unsigned char x318[0x3E7 - 0x318];
    unsigned char x3E7;
    char pad3e8[0x6E8 - 0x3E8];
    char x6E8;
    unsigned char x6E9;
    char pad6ea[0x6F8 - 0x6EA];
    unsigned char x6F8;
    unsigned char x6F9;
    unsigned char x6FA;
    unsigned char x6FB;
    unsigned char x6FC;
    char pad6fd[0xC2C - 0x6FD];
    unsigned char xC2C;
    unsigned char xC2D;
} WorkView0x800315a4;
#define WV_0x800315a4 (*SCRATCH(WorkView0x800315a4 *, 0))
#define SPAD08_0x800315a4 (*SCRATCH(short, 0x8))
#define SPAD0A_0x800315a4 (*SCRATCH(short, 0xA))
extern signed char D_800B2488;
extern short D_800ABA68;
extern unsigned short D_800ABA90;
extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern void func_8001DB64();
extern void func_8001DBFC(void);
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern void func_8004262C(int);
extern void func_8005AD28();
extern void func_8002F398();

void func_800315A4(void)
{
    unsigned char *p;
    int a;

    if (D_800B2488 == 0) {
        D_800B2488++;
        func_8004262C(0);
    } else {
        p = WV_0x800315a4->x318;
        if (WV_0x800315a4->x6E8 != 0 && *(unsigned short *)&WV_0x800315a4->xC2C != 0x905) {
            WV_0x800315a4->x6E9 = 1;
            a = p[0x4C];
            p[0x4C] = p[0x4E];
            WV_0x800315a4->x3E7 = 0xFF;
            func_8001DB64(a);
            func_8001DBFC();
            func_8002EB14(1, 1, 1);
            func_8001CDE4(1);
            WV_0x800315a4->x6E8 = 0;
        }
        if (WV_0x800315a4->xC2D == 7) {
            WV_0x800315a4->x6F8 = 0x70;
            WV_0x800315a4->x6F9 = 0x70;
            WV_0x800315a4->x6FA = 0x70;
            WV_0x800315a4->x6FB = 0;
            WV_0x800315a4->x6FC = 0;
        }
        func_80034C48(2, 0x10, 0);
        func_8002EB14(0, 0xF, 1);
        SPAD08_0x800315a4 = 0;
        SPAD0A_0x800315a4 = 0;
        D_800ABA68 = 0;
        D_800ABA90 = 0;
        D_800B2488 = 0;
        func_8001CED4((int)func_8002F398);
    }
    func_8005AD28();
}
