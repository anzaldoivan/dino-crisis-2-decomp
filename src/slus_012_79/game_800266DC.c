#include "common.h"
#include "dc2.h"

extern void func_8001D89C();
extern void func_80079AA4();

void func_800266DC(short a, short b, short c)
{
    SVECTOR v;
    Cam0x800266dc *cam = (Cam0x800266dc *)WORK->x318;

    cam->x28 = a;
    cam->x2A = b;
    cam->x2C = c;
    func_8001D89C(&cam->x28, cam->x38, &cam->x20, cam->x30);
    v.vx = -cam->x20;
    v.vy = -cam->x22;
    v.vz = -cam->x24;
    func_80079AA4(&v, cam);
}
