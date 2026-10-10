#include "common.h"

int func_800339AC(short *a, short *b, short *out)
{
    int ax0, az0, ax1, az1;
    int bx0, bz0, bx1, bz1;
    int den;
    int na;
    int nb;

    ax0 = a[0];
    ax1 = a[2];
    bz0 = b[1];
    bz1 = b[3];
    bx0 = b[0];
    az0 = a[1];
    az1 = a[3];
    bx1 = b[2];
    den = (ax1 - ax0) * (bz1 - bz0) - (az1 - az0) * (bx1 - bx0);
    ((int *)out)[0] = 0;
    ((int *)out)[1] = 0;
    if (den == 0) {
        return 0;
    }
    na = (az0 - bz0) * (bx1 - bx0) - (ax0 - bx0) * (bz1 - bz0);
    nb = (az0 - bz0) * (ax1 - ax0) - (ax0 - bx0) * (az1 - az0);
    if (((den > 0 && na >= 0 && den >= na) || (den < 0 && na >= den && na <= 0))
        && ((den > 0 && nb >= 0 && den >= nb) || (den < 0 && nb >= den && nb <= 0))) {
        den >>= 10;
        na >>= 10;
        if (den == 0) {
            den = 1;
        }
        na = (na << 10) / den;
        out[0] = ax0 + (((ax1 - ax0) * na) >> 10);
        out[2] = az0 + (((az1 - az0) * na) >> 10);
        return 1;
    }
    return 0;
}
