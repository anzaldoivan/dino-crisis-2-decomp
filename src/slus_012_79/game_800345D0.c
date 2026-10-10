#include "dc2.h"

typedef struct {
    char pad0[0x40];
    SVECTOR pos;
    char pad48[0x5A - 0x48];
    short x5A;
} Obj0x800345d0;
extern int func_800339AC(void *, Seg *, SVECTOR *);
extern int func_8004A0F8(SVECTOR *a, SVECTOR *b);

void func_800345D0(Obj0x800345d0 *obj, SVECTOR *res, unsigned char flags)
{
    Seg seg;
    SVECTOR out;
    Seg box[4];
    SVECTOR hit[4];
    unsigned int dist[4];
    unsigned int best[4];
    unsigned char *p;
    unsigned short *pt;
    unsigned int m;
    unsigned int mx;
    int n, k, i;

    box[0].z1 = 0x7CFF;
    box[1].x1 = 0x7CFF;
    box[2].z1 = -0x7CFF;
    box[3].x1 = -0x7CFF;
    dist[0] = 0;
    dist[1] = 0;
    dist[2] = 0;
    dist[3] = 0;
    best[0] = 0x7FFFFFFF;
    best[1] = 0x7FFFFFFF;
    best[2] = 0x7FFFFFFF;
    best[3] = 0x7FFFFFFF;
    box[0].x0 = obj->pos.vx;
    box[0].z0 = obj->pos.vz;
    box[0].x1 = obj->pos.vx;
    box[1].x0 = obj->pos.vx;
    box[1].z0 = obj->pos.vz;
    box[1].z1 = obj->pos.vz;
    box[2].x0 = obj->pos.vx;
    box[2].z0 = obj->pos.vz;
    box[2].x1 = obj->pos.vx;
    box[3].x0 = obj->pos.vx;
    box[3].z0 = obj->pos.vz;
    box[3].z1 = obj->pos.vz;

    n = WORK->areas->count;
    do {
        n--;
        p = (unsigned char *)WORK->areas + WORK->areas->offs[n];
        if (p[0] == 0) {
            continue;
        }
        if (p[3] & flags) {
            continue;
        }
        if (!((*(unsigned short *)(p + 4) >> obj->x5A) & 1)) {
            continue;
        }
        pt = (unsigned short *)(p + (p[1] * 4 + 8));
        k = p[1];
        do {
            seg.x0 = pt[0];
            seg.z0 = pt[1];
            pt += 2;
            seg.x1 = pt[0];
            seg.z1 = pt[1];
            k--;
            for (i = 0; i < 4; i++) {
                if (func_800339AC(&box[i], &seg, &out)) {
                    m = func_8004A0F8(&obj->pos, &out);
                    dist[i] = m;
                    if (m < best[i]) {
                        best[i] = m;
                        hit[i].vx = out.vx;
                        hit[i].vy = out.vy;
                        hit[i].vz = out.vz;
                    }
                }
            }
        } while (k != 0);
    } while (n != 0);

    k = 0;
    mx = best[0];
    for (i = 1; i < 4; i++) {
        if (mx < best[i]) {
            mx = best[i];
            k = i;
        }
    }
    res->vx = hit[k].vx;
    res->vy = hit[k].vy;
    res->vz = hit[k].vz;
}
