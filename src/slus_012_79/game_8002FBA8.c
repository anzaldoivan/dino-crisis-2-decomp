#include "dc2.h"

typedef struct Unit0x8002fba8 {
    char pad0[0x54];
    unsigned char x54;
    char pad55[0x174 - 0x55];
    unsigned short x174;
} Unit0x8002fba8;
typedef struct Work0x8002fba8 {
    char pad0[0x30C];
    short x30C;
    char pad30e[0x4C7 - 0x30E];
    unsigned char x4C7;
    char pad4c8[0x4E0 - 0x4C8];
    Unit0x8002fba8 *units;
    char pad4e4[0xC2C - 0x4E4];
    unsigned char xC2C;
    unsigned char xC2D;
} Work0x8002fba8;
#define WORK0x8002fba8 (*SCRATCH(Work0x8002fba8 *, 0))
#define SPAD_0A0x8002fba8 (*SCRATCH(unsigned short, 0xA))
extern void func_80018000();
extern void func_80018014();
extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern void func_8002219C();
extern void func_80030670();
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80036E94();
extern void func_80041BB8();
extern void func_8004262C(int);
extern void func_8004579C();
extern void func_80051F8C(int, int, int, int, int);
extern void func_8005AD28();
extern void func_8004B508();
extern void func_8002FA34();
extern void func_80031968();

void func_8002FBA8(void)
{
    int ok;
    int hi;

    if (func_80034CD8(2, 0x1B)) {
        func_8004579C();
        func_80041BB8();
        func_8001CDE4(0x1E);
        func_8004262C(0x11);
    }
    if (!func_80034CD8(2, 6) && !func_80034CD8(2, 0xF)) {
        ok = 1;
        if (func_80034CD8(1, 9) || func_80034CD8(1, 0x10) || WORK0x8002fba8->units->x174 != 0 ||
            func_80034CD8(1, 0xA) || func_80034CD8(2, 0x25) || WORK0x8002fba8->units->x54 != ok) {
            ok = 0;
        }
        if ((SPAD_0A0x8002fba8 & 0x100) && ok) {
            func_80034C48(2, 8, 1);
        } else if ((SPAD_0A0x8002fba8 & 1) && WORK0x8002fba8->x4C7 == 0 && ok) {
            hi = WORK0x8002fba8->xC2D;
            func_80051F8C(6, WORK0x8002fba8->xC2C | (hi << 8), 0, 0, 0);
        } else if (SPAD_0A0x8002fba8 & 0x800) {
            func_80034C48(2, 0x10, 1);
        }
    }
    if (!func_80034CD8(2, 6)) {
        if (func_80034CD8(2, 8)) {
            func_80034C48(2, 0x25, 1);
            func_8005AD28();
            func_8001CED4((int)func_8004B508);
        } else if (func_80034CD8(2, 0x11)) {
            func_8005AD28();
            WORK0x8002fba8->x30C = 0;
            func_8001CED4((int)func_8002FA34);
            return;
        } else if (func_80034CD8(2, 0x10) && !func_80034CD8(2, 0xF)) {
            func_8005AD28();
            func_8001CED4((int)func_80031968);
        }
    }
    if (func_80034CD8(3, 9)) {
        func_8002219C();
    }
    func_80018000(WORK0x8002fba8);
    func_80036E94();
    func_8005AD28();
    func_80018014(WORK0x8002fba8);
    func_80034C48(2, 0x25, 0);
    func_80030670();
}
