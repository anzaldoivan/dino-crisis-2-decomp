#include "common.h"
#include "dc2.h"

int func_80034CD8(int, int);
void func_80034C48(int, int, int);
void func_8001D28C(void);
void func_80063CFC(void);
void func_8005E518(void);
void func_80035CA8(void);
void func_80036AB4(void);
void func_8002E9DC(void);
void func_80035670(void);
void func_800354A8(void);
void func_80035744(void);
void func_800355D4(void);
void func_8002AC1C(void);
void func_80064AB4(void);
void func_8001DBFC(void);
void func_80020ACC(void);
void func_80022778(int, int);
void func_800231F0(int, int);
void func_8004A790(void);
void func_80037C9C(void *);

void func_80036E94(void)
{
    if (!func_80034CD8(2, 0x1D)) {
        func_80034C48(2, 0x19, 0);
        if (!func_80034CD8(1, 1)) {
            func_8001D28C();
        }
        if (!func_80034CD8(1, 8)) {
            func_80063CFC();
        }
        if (!func_80034CD8(1, 6)) {
            func_8005E518();
        }
        if (!func_80034CD8(1, 0xB)) {
            func_80035CA8();
            func_80036AB4();
        }
        func_8002E9DC();
        if (!func_80034CD8(1, 7)) {
            func_80035670();
        }
        func_800354A8();
        if (!func_80034CD8(1, 0xD)) {
            func_80035744();
        }
        if (!func_80034CD8(1, 4)) {
            func_800355D4();
        }
        func_8002AC1C();
        func_80064AB4();
    }
    if (WORK->x3E6 != WORK->x3E7) {
        func_8001DBFC();
        WORK->x3E7 = WORK->x3E6;
    }
    if (!func_80034CD8(1, 0xC)) {
        func_80020ACC();
    }
    func_80022778(0xC0, 0x10);
    if (func_80034CD8(3, 0xB)) {
        func_800231F0(0xC8, 0xD0);
    }
    func_8004A790();
    if (!func_80034CD8(1, 5)) {
        func_80037C9C(WORK->x318);
    }
}
