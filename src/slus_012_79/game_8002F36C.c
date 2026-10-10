#include "dc2.h"

typedef struct Work0x8002f36c {
    char pad0[0x204];
    short x204;
} Work0x8002f36c;
#define WORK0x8002f36c (*SCRATCH(Work0x8002f36c *, 0))
extern void func_80018020(void *, int);

void func_8002F36C(void)
{
    func_80018020(&WORK0x8002f36c->x204, 0x42);
}
