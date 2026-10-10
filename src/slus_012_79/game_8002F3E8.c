#include "common.h"
#include "dc2.h"

extern unsigned short D_800B32F0[];
extern void func_8001D180();
extern void func_8002F080();
extern void func_8003079C();
extern void func_80034C48(int, int, int);
extern int func_80035A88(int a, int b);
extern void func_80043BF8();
extern void func_80064A60();

void func_8002F3E8(void)
{
    unsigned short *d;
    unsigned short i;
    Work *w;

    d = D_800B32F0;
    WORK->x204++;
    func_8001D180();
    func_80064A60();
    func_80043BF8();
    WORK->x214 = WORK->x216 = WORK->x218 = WORK->x21A = WORK->x21C = WORK->x21E = WORK->x220 = WORK->x222 = 0xFFFF;
    d[0x19] = 0xFFFF;
    d[0x18] = 0xFFFF;
    d[0x1B] = 0xFFFF;
    d[0x1A] = 0xFFFF;
    if (WORK->x202) {
        WORK->x3DC = WORK->xC48;
        WORK->x3DE = WORK->xC4A;
        WORK->x3E0 = WORK->xC4C;
        WORK->x3E2 = WORK->xC4E;
    } else {
        func_8002F080();
    }
    w = WORK;
    if (w->x4C7) {
        *(unsigned short *)&w->xC2C = 0x905;
        WORK->x3DC = WORK->x3DE = WORK->x3E0 = WORK->x3E2 = 0;
        WORK->padc2a[0] = 1;
        WORK->xC2B = WORK->x3EC;
        WORK->xE6C[func_80035A88(WORK->xC2D, WORK->xC2C)] = 0;
        func_8003079C();
        for (i = 0; i < 0x30; i++) {
            func_80034C48(9, i + 0x50, 0);
        }
    } else if (w->xC30 & 1) {
        func_80034C48(9, 0x35, 1);
    }
}
