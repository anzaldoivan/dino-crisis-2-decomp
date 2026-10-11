#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x2474];
    unsigned char x2474;
    char pad2475[0x2477 - 0x2475];
    unsigned char x2477;
    char pad2478[0x247D - 0x2478];
    unsigned char x247D;
} Obj0x8002751c;
extern void func_8002EB14(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80044834(void *, int);

void func_8002751C(Obj0x8002751c *p)
{
    Unit *u = WORK->units;

    switch (p->x2477) {
    case 0:
        func_8002EB14(0, 0x14, 1);
        p->x2477++;
    case 1:
        if (func_80034CD8(2, 6) == 0) {
            p->x2477++;
            func_80044834(0, 0x78);
        }
        break;
    case 8:
        if (p->x247D == 0x11) {
            p->x2474++;
        }
        break;
    }
    if (p->x2477 >= 5) {
        u->x44 += 0x1B;
    }
}
