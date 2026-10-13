#include "common.h"
#include "dc2.h"

/* Ent0x8001ef60: dc2.h (T8 w35) */
typedef struct {
    char pad0[0x4C];
    int x4C;
    int x50;
    int x54;
    int x58;
    int x5C;
    int x60;
    char pad64[0x70 - 0x64];
    short x70;
    char pad72[0x7D - 0x72];
    unsigned char x7D;
    char pad7e[0x80 - 0x7E];
    int x80;
    int x84;
} Obj0x8001ef60;
extern Obj0x8001ef60 D_800AF5C0;
extern Ent0x8001ef60 D_80085FB0[];
extern int func_8001EC10(int);

int func_8001EF60(int a0, int a1)
{
    Ent0x8001ef60 *e = &((Ent0x8001ef60 *)WORK->x60C)[(unsigned short)a1];
    unsigned short id = a0;
    int v0 = e->x0;
    int v1 = e->x4;
    Obj0x8001ef60 *o = &D_800AF5C0;
    int t;

    if (func_8001EC10(id) != 0) {
        t = D_80085FB0[id].x0;
        o->x7D = 3;
        o->x80 = id;
        o->x84 = 0;
        o->x58 = v1;
        o->x60 = v1;
        o->x5C = v1;
        o->x70 = v0;
        t += v0;
        o->x4C = t;
        o->x54 = t;
        o->x50 = t;
        return 1;
    }
    return 0;
}
