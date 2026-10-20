#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x20];
    SVECTOR x20;
    SVECTOR x28;
    SVECTOR x30;
} Cam0x8001d180;
extern void func_80018020(void *, int);

void func_8001D180(void)
{
    Cam0x8001d180 *c = (Cam0x8001d180 *)WORK->x318;

    func_80018020(c, 0x22);
    c->x20.vx = -0x100;
    c->x20.vy = 0;
    c->x20.vz = 0;
    c->x28.vx = 0;
    c->x28.vy = -0x5DC;
    c->x28.vz = 0;
    c->x30.vx = 0;
    c->x30.vy = 0;
    c->x30.vz = 0x1000;
}
