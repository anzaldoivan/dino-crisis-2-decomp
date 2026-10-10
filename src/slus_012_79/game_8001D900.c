#include "dc2.h"

typedef struct {
    char pad0[0x200];
    unsigned char x200;
    unsigned char x201;
} Work0x8001d900;
#define WORK0x8001d900 (*SCRATCH(Work0x8001d900 *, 0))
extern void func_8007E234(int);
extern void func_8007E6C0(void);

void func_8001D900(void)
{
    func_8007E234(0);
    func_8007E6C0();
    WORK0x8001d900->x200 = 0;
    WORK0x8001d900->x201 = 0;
}
