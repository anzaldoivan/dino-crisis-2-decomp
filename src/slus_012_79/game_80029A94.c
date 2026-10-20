#include "common.h"

#include "dc2.h"
extern void func_800296E4();

void func_80029A94(void)
{
    int y = 0xAE;

    if (WORK->x7C0 != 0) {
        y = 0x96;
    }
    func_800296E4(0xCD, y, 0, 0xC0, 0, 0x808080);
}
