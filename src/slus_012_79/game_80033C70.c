#include "common.h"

typedef struct {
    short x;
    short y;
    short z;
} Vec0x80033c70;

int func_80033C70(Vec0x80033c70 *out, Vec0x80033c70 *a, Vec0x80033c70 *b, int y)
{
    int t;

    if (b->y == a->y) {
        return -1;
    }
    if (a->y < y && b->y < y) {
        return -1;
    }
    if (y < a->y && y < b->y) {
        return -1;
    }
    t = (y - a->y) / (b->y - a->y);
    out->x = a->x + t * (b->x - a->x);
    out->y = y;
    out->z = a->z + t * (b->z - a->z);
    return 0;
}
