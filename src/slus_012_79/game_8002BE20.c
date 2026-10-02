#include "common.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { int w[5]; } M20;
extern int func_80079154(int);
extern void func_80079704(MATRIX *, SVECTOR *, SVECTOR *);
extern void func_80079FC4(SVECTOR *, MATRIX *);
extern int func_8007AC34(int, int);

void func_8002BE20(MATRIX *src, short *ang)
{
    SVECTOR sv;
    SVECTOR out;
    MATRIX mat;

    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    *(int *)&sv.vx = 0;
    sv.vz = 0x1000;
    *(M20 *)&mat = *(M20 *)src;
    func_80079704(&mat, &sv, &out);
    ang[0] -= func_8007AC34(out.vy, out.vz);
    ang[1] += func_8007AC34(out.vx, (short)func_80079154((out.vz * out.vz + out.vy * out.vy) >> 12));
    sv.vz = 0;
    sv.vx = 0;
    sv.vy = 0x1000;
    func_80079704(&mat, &sv, &out);
    sv.vz = 0;
    sv.vx = -ang[0];
    sv.vy = -ang[1];
    func_80079FC4(&sv, &mat);
    func_80079704(&mat, &out, &sv);
    ang[2] -= func_8007AC34(sv.vx, sv.vy);
}
