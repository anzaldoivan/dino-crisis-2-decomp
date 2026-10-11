#include "common.h"
#include "dc2.h"

typedef struct { short vx, vy, vz, pad; } SVec0x80035aac;

void func_80035AAC(Pt0x80034468 *a, Pt0x80034468 *b, SVec0x80035aac *out)
{
    VECTOR v[2];

    v[0].vx = (a[0].x + a[1].x + a[2].x + a[3].x) / 4;
    v[0].vz = (a[0].z + a[1].z + a[2].z + a[3].z) / 4;
    v[1].vx = (b[0].x + b[1].x + b[2].x + b[3].x) / 4;
    v[1].vz = (b[0].z + b[1].z + b[2].z + b[3].z) / 4;
    out->pad = 0;
    out->vy = 0;
    out->vx = v[1].vx - v[0].vx;
    out->vz = v[1].vz - v[0].vz;
}
