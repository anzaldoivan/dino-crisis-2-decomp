#include "dc2.h"

typedef struct Obj0x80029458 {
    char pad0[0x2475];
    unsigned char x2475;
    unsigned char x2476;
} Obj0x80029458;
extern Grp0x80029458 D_80089B08[];
extern int func_8001D9D4(void);
extern void func_8001EC10(int);

void func_80029458(Obj0x80029458 *a0)
{
    Elem0x80029458 *e;

    e = &D_80089B08[WORK->xC2D].xC[WORK->xC2C];
    switch (a0->x2476) {
    case 0:
        if (func_8001D9D4()) {
            return;
        }
        a0->x2476++;
        func_8001EC10(e->x2);
    case 1:
        if (func_8001D9D4()) {
            return;
        }
        WORK->x21E = e->x2;
        a0->x2476 = 0;
        a0->x2475++;
        break;
    }
}
