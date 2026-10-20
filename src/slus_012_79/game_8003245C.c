#include "common.h"

extern int func_80078E14(int);

int func_8003245C(short *a, short *b, short h0, short h1)
{
    int dx;
    int dz;
    int s;
    int dy;

    dx = a[0] - b[0];
    dz = a[2] - b[2];
    s = a[3] + b[3];
    s -= func_80078E14(dx * dx + dz * dz);
    if (s >= 0) {
        s = h0 + h1;
        dy = a[1] - b[1];
        s -= (dy < 0) ? -dy : dy;
        if (s >= 0) {
            return 1;
        }
    }
    return 0;
}
