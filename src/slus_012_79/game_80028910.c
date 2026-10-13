#include "common.h"
#include "dc2.h"

typedef struct Obj0x80028910 {
    char pad0[0x530];
    Unit units[3];
    char xC50[0x249C - 0xC50];
    int x249C;
} Obj0x80028910;
extern int D_800ABA6C;
extern int D_800AF11C;
extern void func_80047B8C(void *, void *);

void func_80028910(Obj0x80028910 *obj, unsigned char idx, unsigned short *pos)
{
    Unit *u;
    Unit *w;
    void *arg;

    u = WORK->units;
    arg = (void *)u->x60;
    obj->x249C = u->x88;
    WORK->units = &obj->units[idx];
    *(void **)WORK->x4D8 = obj->xC50;
    D_800ABA6C = 0x1E;
    w = WORK->units;
    w->flags = 3;
    w->x40 = pos[0];
    w->x42 = pos[1];
    w->x44 = pos[2];
    w->x48.vz = 0;
    w->x48.vy = 0;
    w->x48.vx = 0;
    w->xF4 = 0;
    w->xF2 = 0;
    w->xF0 = 0;
    w->x5A = 0;
    w->x88 = obj->x249C;
    D_800AF11C = 1;
    func_80047B8C(w, arg);
}
