#include "dc2.h"

typedef struct {
    char pad0[0xC29];
    unsigned char xC29;
} Work0x8001d134;
#define WORK0x8001d134 (*SCRATCH(Work0x8001d134 *, 0))
extern void func_8001CED4(int);
extern void func_8004262C(int);
extern void func_800591B0(void);

void func_8001D134(void)
{
    if (WORK0x8001d134->xC29 != 0) {
        func_8004262C(0x16);
    } else {
        func_8001CED4((int)func_800591B0);
    }
}
