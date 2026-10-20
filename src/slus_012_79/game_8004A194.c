#include "common.h"

extern int func_8004A144(int v);

int func_8004A194(short *a, short *b)
{
    int dx, dy, dz;

    {
        int p = a[0], q = b[0];
        dx = q - p;
        if (p - q >= 0) dx = p - q;
    }
    {
        int p = a[1], q = b[1];
        dy = q - p;
        if (p - q >= 0) dy = p - q;
    }
    {
        int p = a[2], q = b[2];
        dz = q - p;
        if (p - q >= 0) dz = p - q;
    }
    return func_8004A144(dx * dx + dy * dy + dz * dz);
}
