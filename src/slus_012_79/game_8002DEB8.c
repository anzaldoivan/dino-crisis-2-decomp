#include "common.h"

typedef struct {
    char pad0[0x54];
    unsigned char x54;
    char pad55[0x7A - 0x55];
    unsigned char x7A;
} Obj0x8002deb8;
extern void (*D_80089168[])(Obj0x8002deb8 *);
extern void func_8004A66C(Obj0x8002deb8 *);
extern int func_80034CD8(int, int);
extern void func_80047814(Obj0x8002deb8 *);

void func_8002DEB8(Obj0x8002deb8 *p)
{
    D_80089168[p->x54](p);
    func_8004A66C(p);
    if (p->x7A == 0 && func_80034CD8(2, 0x18) != 0) {
        func_80047814(p);
    }
}
