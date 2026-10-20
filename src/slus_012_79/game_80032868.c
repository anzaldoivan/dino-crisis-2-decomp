#include "common.h"

#include "dc2.h"
#include "gte.h"

int func_80032868(short *a, short *b, short *c)
{
    volatile VECTOR v0;
    volatile VECTOR v1;
    volatile VECTOR *p;

    v1.vy = 0;
    v0.vy = 0;
    v0.vx = a[0] - c[0];
    v0.vz = a[1] - c[1];
    v1.vx = b[0] - c[0];
    v1.vz = b[1] - c[1];
    GTE_CTC2_024(&v0);
    p = &v1;
    GTE_LWC2(11, p->vz);
    GTE_LWC2(9, p->vx);
    GTE_LWC2(10, p->vy);
    GTE_OP(0);
    p = &v1;
    GTE_SWC2_AT(25, 0, p);
    GTE_SWC2_AT(26, 4, p);
    GTE_SWC2_AT(27, 8, p);
    return v1.vy >= 0;
}
