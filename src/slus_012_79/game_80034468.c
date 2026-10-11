#include "dc2.h"

/* Pt0x80034468: dc2.h (T8 w35) */
typedef struct {
    unsigned char pad0;
    unsigned char n;
    char pad2[6];
    Pt0x80034468 pts[1];
} Poly0x80034468;
int func_80032908(Seg *a, Seg *b);

short func_80034468(Poly0x80034468 *p, Seg *pt)
{
    short count;
    Seg seg;
    Pt0x80034468 *pp;
    int n;
    int wrap;
    int in;

    count = 0;
    n = p->n;
    wrap = 2;
    pp = &p->pts[n];
    do {
        seg.x0 = pp->x;
        n--;
        seg.z0 = pp->z;
        pp++;
        seg.x1 = pp->x;
        seg.z1 = pp->z;
        if (func_80032908(pt, &seg)) {
            if (seg.x0 == pt->x0) continue;
            if (seg.x1 == pt->x0) {
            again:
                if (wrap != 0) {
                    if (n == 0) {
                        n = p->n;
                        wrap--;
                        pp = &p->pts[n];
                    }
                    pp++;
                    seg.x1 = pp->x;
                    seg.z1 = pp->z;
                    n--;
                    if (seg.x1 == pt->x0) goto again;
                }
                if (seg.x0 > pt->x0) {
                    in = seg.x1 < pt->x0;
                } else {
                    in = pt->x0 < seg.x1;
                }
                if (in) count++;
            } else {
                count++;
            }
        }
        if (wrap != 2) break;
    } while (n != 0);
    return count;
}
