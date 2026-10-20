#include "common.h"
#include "dc2.h"

extern int func_80078E14();
extern int func_8004A2F8();
extern void func_8007A824();
extern SVECTOR *func_80079704();

int func_80033B14(short *line, short *pos, SVECTOR *v, SVECTOR *out)
{
    MATRIX m;
    int a;
    int b;
    int c;
    int d;
    int len;
    int base;
    int z;

    a = line[3] - line[1] + 1;
    b = line[0] - line[2] + 1;
    c = line[2] * line[1] - line[0] * line[3];
    d = a * pos[0] + b * pos[2] + c;
    base = pos[3] + 4;
    len = func_80078E14(a * a + b * b);
    z = (d >= 0 ? d : -d) / len;
    if (d < 0) {
        z = base - z;
    } else {
        z = z + base;
    }
    v->vz = 0;
    v->vx = 0;
    v->vy = (func_8004A2F8(line) - 0x400) & 0xFFF;
    func_8007A824(v, &m);
    v->vy = 0;
    v->vz = base;
    func_80079704(&m, v, out);
    v->vz = z;
    func_80079704(&m, v, v);
    return 1;
}
