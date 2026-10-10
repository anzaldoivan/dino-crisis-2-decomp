#include "dc2.h"

typedef struct Obj0x800292f4 {
    char pad0[0x2475];
    unsigned char x2475;
    unsigned char x2476;
    char pad2477;
    unsigned char x2478;
    unsigned char x2479;
} Obj0x800292f4;
typedef struct Ent0x800292f4 {
    int x0;
    unsigned short x4;
    char pad6[0x10 - 0x6];
    void (*x10)(void);
    int x14;
} Ent0x800292f4;
extern Ent0x800292f4 D_80089B08[];
extern int D_800181D4;
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);

void func_800292F4(Obj0x800292f4 *obj)
{
    switch (obj->x2476) {
    case 0:
        obj->x2476 = 1;
        if (WORK->x21C == obj->x2479) {
            break;
        }
        return;
    case 1:
        if (func_8001D9D4() != 0) {
            return;
        }
        obj->x2476++;
        func_8001ECBC(D_80089B08[obj->x2479].x4, D_800181D4);
    case 2:
        if (func_8001D9D4() != 0) {
            return;
        }
        break;
    default:
        return;
    }
    WORK->xC45 = WORK->xC2D;
    WORK->xC2D = obj->x2479;
    WORK->xC44 = WORK->xC2C;
    WORK->xC2C = obj->x2478;
    D_80089B08[WORK->xC2D].x10();
    obj->x2476 = 0;
    obj->x2475++;
}
