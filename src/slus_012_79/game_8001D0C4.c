#include "common.h"

extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);
extern char D_800D5920[];

void func_8001D0C4(void)
{
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001ECBC(2, 0x800D5800);
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001CED4((int)D_800D5920);
}
