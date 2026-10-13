#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned int *bits;
    int pad;
} Flags0x80034c48;
extern Flags0x80034c48 D_80089218[];
extern unsigned short D_800ABA90;
extern unsigned short D_800ABA68;

void func_80034C48(int set, unsigned int bit, int on)
{
    unsigned int *p;

    p = ((Flags0x80034c48 *)((char *)D_80089218 + (set << 3)))->bits;
    p += bit >> 5;

    if (on == 0) {
        *p &= ~(1 << (bit & 0x1F));
        return;
    }
    *p |= 1 << (bit & 0x1F);
    if (set == 1 && bit == 0) {
        *SCRATCH(unsigned short, 0xA) = 0;
        *SCRATCH(unsigned short, 0x8) = 0;
        D_800ABA90 = 0;
        D_800ABA68 = 0;
    }
}
