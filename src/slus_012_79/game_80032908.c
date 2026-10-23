#include "common.h"

#include "dc2.h"

int func_80032908(Seg *a, int *p)
{
    Seg *b = (Seg *)p;
    int ax0, ax1, bx0, bx1;
    int az0, az1, bz0, bz1;
    int dax, dbz, daz, dbx;
    int d, s, t, ox, oz;

    ax0 = a->x0;
    ax1 = a->x1;
    bx0 = b->x0;
    bx1 = b->x1;
    if (ax1 < ax0) {
        if (bx0 < bx1) {
            if (ax0 < bx0) return 0;
            if (bx1 < ax1) return 0;
        } else {
            if (ax0 < bx1) return 0;
            if (bx0 < ax1) return 0;
        }
    } else {
        if (bx0 < bx1) {
            if (ax1 < bx0) return 0;
            if (bx1 < ax0) return 0;
        } else {
            if (ax1 < bx1) return 0;
            if (bx0 < ax0) return 0;
        }
    }
    az0 = a->z0;
    az1 = a->z1;
    bz0 = b->z0;
    bz1 = b->z1;
    if (az1 < az0) {
        if (bz0 < bz1) {
            if (az0 < bz0) return 0;
            if (bz1 < az1) return 0;
        } else {
            if (az0 < bz1) return 0;
            if (bz0 < az1) return 0;
        }
    } else {
        if (bz0 < bz1) {
            if (az1 < bz0) return 0;
            if (bz1 < az0) return 0;
        } else {
            if (az1 < bz1) return 0;
            if (bz0 < az0) return 0;
        }
    }
    dax = ax1 - ax0;
    dbz = bz1 - bz0;
    daz = az1 - az0;
    dbx = bx1 - bx0;
    d = dax * dbz - daz * dbx;
    if (d == 0) return 0;
    oz = az0 - bz0;
    ox = ax0 - bx0;
    s = oz * dbx - ox * dbz;
    t = oz * dax - ox * daz;
    if (!((d > 0 && s >= 0 && s <= d) || (d < 0 && s >= d && s <= 0))) return 0;
    if (!((d > 0 && t >= 0 && t <= d) || (d < 0 && t >= d && t <= 0))) return 0;
    return 1;
}
