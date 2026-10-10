#include "dc2.h"

typedef struct {
    char pad0[0x318];
    Sub0x8001d230 x318;
    char pad367[0x3E6 - 0x367];
    unsigned char x3E6;
    char pad3e7[0x6E9 - 0x3E7];
    unsigned char x6E9;
} Work0x8001db64;
#define WORK0x8001db64 (*SCRATCH(Work0x8001db64 *, 0))
extern void func_80034C48(int, int, int);

void func_8001DB64(unsigned short arg0)
{
    Sub0x8001d230 *s = &WORK0x8001db64->x318;

    s->x4E = s->x4C;
    s->x4C = arg0;
    WORK0x8001db64->x3E6 = arg0;
    if (WORK0x8001db64->x6E9 == 0) {
        func_80034C48(2, 0x18, 1);
    }
    func_80034C48(2, 1, 0);
    func_80034C48(2, 13, 0);
    func_80034C48(2, 2, 1);
}
