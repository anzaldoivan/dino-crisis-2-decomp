#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x4C];
    unsigned char x4C;
} Sub0x8001d28c;
extern int func_80034CD8(int, int);
extern void func_8001D338();

void func_8001D28C(void)
{
    Sub0x8001d28c *s = (Sub0x8001d28c *)WORK->x318;

    if (func_80034CD8(1, 10) == 0) {
        func_8001D338(s, WORK->x5E4);
    }
}
