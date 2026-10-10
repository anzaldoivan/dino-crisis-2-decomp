#include "common.h"
#include "dc2.h"

typedef struct { short a; short pad2; int pad4; int b; } Sub;
typedef struct {
    char pad0[0x20];
    char m20[0x20];
    unsigned short x40; short pad42;
    unsigned short x44; char pad46[0xA];
    int x50;
    unsigned char x54, x55, x56; char pad57[0xB1];
    short x108; short pad10A;
    unsigned short x10C; char pad10E[0x1E];
    void *x12C; int pad130;
    void *x134; char pad138[0x88];
    Sub sub; char pad1CC[0x28];
    short x1F4[8];
} Obj800D5990;
extern int D_800D94D0[];
extern int func_80034CD8(int, int);
extern void func_800484F0(Obj800D5990 *, int, int, int, int);
extern void func_80079704(void *, SVECTOR *, SVECTOR *);
extern void func_800D6204(Obj800D5990 *, int);

void func_800D5990(Obj800D5990 *p) {
    Sub *sub = &p->sub;
    Unit *ctx;
    SVECTOR v[4];
    SVECTOR *vp;
    short *out;
    short *pz;
    unsigned short x, z;
    int i;
    char *m;
    int *tbl;

    ctx = WORK->units;
    p->x54 += 3;
    p->x55 = 0;
    p->x56 = 0;
    if (func_80034CD8(6, sub->a)) {
        p->x54--;
        return;
    }
    p->x108 = 0;
    ctx->flags |= 0x80;
    p->x50 = 0x1D;
    p->x12C = D_800D94D0;
    p->x134 = D_800D94D0 + 3;
    func_800D6204(p, sub->b);
    func_800484F0(p, 0, 0, 0, 0);
    out = p->x1F4;
    v[0].vx = 300;
    v[0].vy = 0;
    v[0].vz = 0;
    v[1].vx = 800;
    v[1].vy = 0;
    v[1].vz = -0x8FC;
    v[2].vx = -800;
    v[2].vz = -0x8FC;
    v[3].vx = -300;
    v[3].vy = 0;
    v[3].vz = 0;
    if (p->x10C) {
        v[1].vz = -0x11F8;
        v[2].vz = -0x11F8;
    }
    m = p->m20;
    pz = &v[0].vz;
    vp = v;
    for (i = 3; i >= 0; i--) {
        func_80079704(m, vp, vp);
        z = *pz;
        pz += 4;
        x = vp->vx;
        vp++;
        *out++ = x + p->x40;
        *out++ = z + p->x44;
    }
}
