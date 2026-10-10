#include "dc2.h"

typedef struct {
    char pad0[0x7C6];
    unsigned char x7C6;
    unsigned char x7C7;
} Work0x80031084;
#define WORK0x80031084 (*SCRATCH(Work0x80031084 *, 0))
extern void func_80079924(int, int);

void func_80031084(void)
{
    Work0x80031084 *w = WORK0x80031084;

    w->x7C7 = 0;
    w->x7C6 = 0;
    func_80079924(0xA0, 0x78);
}
