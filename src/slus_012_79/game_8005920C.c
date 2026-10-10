#include "dc2.h"

typedef struct {
    char pad0[0x30C];
    unsigned short x30C;
} Sub0x8005920c;
extern void func_80018020(void *, int);
extern void func_80034C48(int, int, int);

void func_8005920C(void *p)
{
    func_80018020(p, 0x10);
    ((Sub0x8005920c *)WORK)->x30C++;
    func_80034C48(3, 0, 0);
}
