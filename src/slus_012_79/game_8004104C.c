#include "dc2.h"

extern S800B27E0 D_800B27E0;
typedef struct Src0x8004104c {
    char pad0[0x48];
    SVECTOR x48;
} Src0x8004104c;
typedef struct Shot0x8004104c {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    signed char x46;
    signed char x47;
    short x48;
    char pad4a[0x4E - 0x4A];
    signed char x4E;
    signed char x4F;
    int x50;
    int x54;
    short x58;
    short x5A;
    short x5C;
    char pad5e[0x68 - 0x5E];
    int x68;
} Shot0x8004104c;
typedef struct Ent0x8004104c {
    char pad0[0x58];
    unsigned char x58;
    char pad59[0x60 - 0x59];
    void *x60;
    unsigned char x64;
    unsigned char x65;
    unsigned char x66;
    char pad67[0x6C - 0x67];
    short x6C;
    short x6E;
    char pad70[0x7A - 0x70];
    unsigned char x7A;
    char pad7b[0x84 - 0x7B];
    unsigned char x84;
} Ent0x8004104c;
extern void *func_800402EC(void);
extern void *func_80047714(void);
void func_80018020(void *, int);
unsigned int func_8003252C(void *);
void func_80047958(void *);
int func_8004A32C(void);
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007AA34(SVECTOR *, MATRIX *);

int func_8004104C(Src0x8004104c *src, short *pos)
{
    SVECTOR v;
    SVECTOR out;
    MATRIX m;
    Shot0x8004104c *p;
    Ent0x8004104c *e;
    unsigned char i;

    p = func_800402EC();
    if (p == 0) {
        return 0;
    }
    func_8007AA34(&src->x48, &m);
    *(int *)&v.vx = 0;
    v.vz = 0x4B0;
    func_80079704(&m, &v, &out);
    p->x40 = pos[0];
    p->x42 = pos[1];
    p->x44 = pos[2];
    func_80018020(&D_800B27E0, 0x2B);
    for (i = 0;;) {
        p->x68 = 0x8017CA14;
        p->x50 = 1;
        p->x54 = 0;
        p->x4E = 3;
        p->x4F = 3;
        *(int *)&p->x58 = *(int *)&p->x40;
        p->x5C = p->x44;
        if (i != 0) {
            p->x40 += out.vx + (signed char)func_8004A32C() * 4;
            p->x42 += out.vy;
            p->x44 += out.vz + (signed char)func_8004A32C() * 4;
        }
        p->x48 = 0x800;
        p->x46 = i;
        p->x47 = i * 3 + 2;
        D_800B27E0.x50 = 1;
        D_800B27E0.x5A = p->x42 / -1000;
        D_800B27E0.x85 = 0;
        *(int *)&D_800B27E0.x40 = *(int *)&p->x40;
        D_800B27E0.x44 = p->x44;
        *(int *)&D_800B27E0.x74 = *(int *)&p->x58;
        D_800B27E0.x78 = p->x5C;
        if (func_8003252C(&D_800B27E0) != 0) {
            func_80047958(p);
            return i;
        }
        e = func_80047714();
        if (e != 0) {
            e->x6C = e->x6E = 0x14;
            e->x58 = 2;
            e->x7A = 3;
            e->x64 = 0;
            e->x65 = 0;
            e->x66 = 0xFF;
            e->x60 = p;
            e->x84 |= 8;
        }
        i++;
        *(int *)&v.vx = *(int *)&p->x40;
        v.vz = p->x44;
        if (i >= 6) {
            return i;
        }
        p = func_800402EC();
        if (p == 0) {
            return i;
        }
        *(int *)&p->x40 = *(int *)&v.vx;
        p->x44 = v.vz;
    }
}
