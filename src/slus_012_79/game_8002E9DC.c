#include "common.h"
#include "dc2.h"

typedef struct {
    short v;
    unsigned char state;
} Fade0x8002e9dc;
extern int func_80034CD8(int, int);

void func_8002E9DC(void)
{
    Work *w = WORK;
    Fade0x8002e9dc *f = (Fade0x8002e9dc *)&w->x6B0;
    int v;

    switch (f->state) {
    case 0:
        if (func_80034CD8(2, 7) == 0) {
            break;
        }
        f->state++;
        w->x6B0 = 0;
    case 1:
        v = f->v + 6;
        if (v >= 0x100) {
            v = 0xFF;
        }
        f->v = v;
        if (f->v >= 0xFF) {
            f->state++;
        }
        break;
    case 2:
        if (func_80034CD8(2, 7) != 0) {
            break;
        }
        f->state++;
        w->x6B0 = 0xFF;
    case 3:
        v = f->v - 20;
        if (v < 0) {
            v = 0;
        }
        f->v = v;
        if (f->v <= 0) {
            f->state = 0;
        }
        break;
    }
}
