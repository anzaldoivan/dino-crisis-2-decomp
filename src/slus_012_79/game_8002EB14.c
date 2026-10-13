#include "common.h"
#include "dc2.h"

typedef struct {
    short x0;
    short x2;
    short x4;
    short x6;
    unsigned int x8;
    int xC;
    signed char x10;
    unsigned char x11;
    unsigned char x12;
} Fade0x8002eb14;
extern void func_80034C48(int, int, int);

void func_8002EB14(int a, unsigned int b, int c)
{
    Fade0x8002eb14 *p = (Fade0x8002eb14 *)(WORK->x318 + 0x88);

    if (p->x10 != a) {
        switch (a) {
        case 0:
            p->xC = 0xFF0000;
            break;
        case 1:
            p->xC = 0;
            break;
        }
        func_80034C48(2, 6, 1);
        p->x11 = b - 1;
        p->x8 = 0x1000000 / b;
        p->x10 = a;
        p->x12 = c;
        p->x0 = 0;
        p->x2 = 0;
        p->x4 = 320;
        p->x6 = 240;
    }
}
