#include "dc2.h"

typedef struct {
    char pad0[0x40];
    SVECTOR pos;
    char pad48[0x4A - 0x48];
    unsigned short x4A;
    char pad4C[0x5A - 0x4C];
    short x5A;
} Obj0x800348d0;
/* Vec0x800348d0: VECTOR in dc2.h (T8 w35) */
extern void func_80049F7C();
extern int func_800339AC(void *, Seg *, SVECTOR *);
extern int func_8004A0F8(SVECTOR *a, SVECTOR *b);

unsigned int func_800348D0(Obj0x800348d0 *obj, short ang, unsigned char flags)
{
    Seg seg;
    SVECTOR out;
    Seg box;
    VECTOR end;
    SVECTOR dir;
    unsigned char *p;
    unsigned short *pt;
    unsigned int m;
    unsigned int best;
    int n, k;

    dir.vx = 0;
    dir.vy = 0;
    dir.vz = 30000;
    func_80049F7C(&dir, &dir, (ang + obj->x4A) & 0xFFF);
    end.vx = obj->pos.vx + dir.vx;
    end.vz = obj->pos.vz + dir.vz;
    if (end.vx > 0x7C9C) {
        end.vx = 0x7C9C;
    }
    if (end.vx < -0x7C9C) {
        end.vx = -0x7C9C;
    }
    if (end.vz > 0x7C9C) {
        end.vz = 0x7C9C;
    }
    if (end.vz < -0x7C9C) {
        end.vz = -0x7C9C;
    }
    box.x0 = obj->pos.vx;
    box.z0 = obj->pos.vz;
    box.x1 = end.vx;
    box.z1 = end.vz;

    best = 0x7FFFFFFF;
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
            k--;
            seg.x0 = pt[0];
            seg.z0 = pt[1];
            pt += 2;
            seg.x1 = pt[0];
            seg.z1 = pt[1];
            if (func_800339AC(&box, &seg, &out)) {
                m = func_8004A0F8(&obj->pos, &out);
                if (m < best) {
                    best = m;
                }
            }
        } while (k != 0);
    } while (n != 0);
    return best;
}
