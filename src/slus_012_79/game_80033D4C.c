#include "common.h"
#include "dc2.h"

extern short func_80033F10();

int func_80033D4C(SVECTOR *a, SVECTOR *b, SVECTOR *out, unsigned char c)
{
    int flags;
    int y;

    if ((short)(a->vy / 100) == (short)(b->vy / 100)) {
        return -1;
    }
    y = func_80033F10(a, b, c, &flags);
    if ((a->vy >= b->vy) ? (a->vy < y) : (y < a->vy)) {
        return -1;
    }
    if (a->vy == b->vy) {
        if (y == a->vy) {
            out->vx = b->vx;
            out->vy = b->vy;
            out->vz = b->vz;
            if (flags & 2) {
                return -2;
            }
            return 0;
        }
        return -1;
    }
    out->vx = b->vx + ((y - b->vy) * (a->vx - b->vx)) / (a->vy - b->vy);
    out->vz = b->vz + ((y - b->vy) * (a->vz - b->vz)) / (a->vy - b->vy);
    out->vy = y;
    if (flags & 2) {
        return -2;
    }
    return 0;
}
