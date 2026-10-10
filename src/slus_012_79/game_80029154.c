#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x2475];
    unsigned char x2475;
    char pad2476[0x247D - 0x2476];
    unsigned char x247D;
    unsigned char x247E;
    char pad247f[1];
    unsigned char x2480;
    char pad2481[0x2492 - 0x2481];
    unsigned char x2492;
    char pad2493[1];
    unsigned short x2494;
    char pad2496[2];
    char *x2498;
} Obj0x80029154;
typedef struct {
    char pad0[0xC];
    void (*fn)(Obj0x80029154 *);
    char pad10[0x20 - 0x10];
} Mode0x80029154;
extern Mode0x80029154 D_80087FC8[];
extern void (*D_80088DA8[])(Obj0x80029154 *);
extern unsigned int D_800AF11C;
extern int func_80034CD8(int, int);
extern int func_800487B8();
extern void func_80029C14();
extern void func_80037C9C(void *);
extern void func_8004AC08();
extern void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
extern void func_80079AA4(SVECTOR *r, MATRIX *m);

void func_80029154(Obj0x80029154 *obj)
{
    Unit *u;
    unsigned int i;

    u = WORK->units;
    if (func_80034CD8(1, 3) == 0) {
        obj->x247E = func_800487B8(u);
    }
    D_80087FC8[obj->x2480].fn(obj);
    D_80088DA8[obj->x2475](obj);
    if (obj->x2480 == 0x58) {
        func_800506BC(obj->x2498, obj->x2494, 0x68, 0x64, 0x808080, 5, 0);
    } else if (obj->x2480 != 0x59) {
        func_800506BC(obj->x2498, 0x10, 0x18, 0x64, 0x808080, 5, 0);
    }
    if (obj->x2492) {
        func_80029C14(obj);
    } else {
        obj->x247D |= 0x10;
    }
    for (i = 0; i < D_800AF11C; u++) {
        func_80079AA4(&u->x48, &u->x20);
        func_8004AC08(u);
        i++;
    }
    func_80037C9C(WORK->x318);
}
