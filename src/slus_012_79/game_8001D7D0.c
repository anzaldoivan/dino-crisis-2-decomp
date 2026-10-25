#include "common.h"

#include "dc2.h"
extern int func_80079D34();
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);

void func_8001D7D0(SVECTOR *base, SVECTOR *ofs, SVECTOR *rot, SVECTOR *out)
{
    SVECTOR v;
    MATRIX m;

    v.vx = rot->vx;
    v.vy = rot->vy;
    v.vz = rot->vz;
    func_80079D34(&v, &m);
    v.vx = -ofs->vx;
    v.vy = -ofs->vy;
    v.vz = -ofs->vz;
    func_80079704(&m, &v, &v);
    out->vx = base->vx + v.vx;
    out->vy = base->vy + v.vy;
    out->vz = base->vz + v.vz;
}
