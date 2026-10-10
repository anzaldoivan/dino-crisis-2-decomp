#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned char flags;
    unsigned char count;
    unsigned char pad2;
    unsigned char mask;
} Area0x80034afc;
int func_80032908(Seg *a, Seg *b);

int func_80034AFC(short *pos, int unused, unsigned char mask)
{
    Seg ray;
    Seg seg;
    int i;
    int j;
    int m;
    short hits;
    short *p;
    AreaTbl *t;
    Area0x80034afc *e;

    ray.x0 = pos[0];
    ray.z0 = pos[2];
    ray.x1 = pos[0];
    ray.z1 = 0x7CFF;
    m = mask | 1;
    i = WORK->areas->count;
    do {
        i--;
        t = WORK->areas;
        e = (Area0x80034afc *)((char *)t + t->offs[i]);
        if (e->flags == 0) continue;
        if (e->mask & m) continue;
        hits = 0;
        p = (short *)((char *)e + (e->count * 4 + 8));
        j = e->count;
        do {
            j--;
            seg.x0 = p[0];
            seg.z0 = p[1];
            p += 2;
            seg.x1 = p[0];
            seg.z1 = p[1];
            if (func_80032908(&ray, &seg)) {
                hits++;
            }
        } while (j != 0);
        if (hits & 1) return 0;
    } while (i != 0);
    return 1;
}
