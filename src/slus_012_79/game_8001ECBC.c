#include "common.h"

typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    char pad24[0x64 - 0x24];
    unsigned short x64;
    char pad66[0x7C - 0x66];
    unsigned char x7C;
    unsigned char x7D;
    char pad7e[0x80 - 0x7E];
    int x80;
    int x84;
} Obj0x8001ecbc;
typedef struct {
    int x0;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
} Hdr0x8001ecbc;
typedef struct {
    int x0;
    int x4;
} Ent0x8001ecbc;
typedef struct { int a[5]; } V5_0x8001ecbc;
extern Obj0x8001ecbc D_800AF5C0;
extern Ent0x8001ecbc D_80085FB0[];
extern int func_8001EC10(int);

int func_8001ECBC(int a0, int a1)
{
    unsigned short id = a0;
    Obj0x8001ecbc *o = &D_800AF5C0;
    int addr;
    Hdr0x8001ecbc *h;
    int v;

    if (func_8001EC10(id) != 0) {
        addr = 0x801E6D00;
        h = (Hdr0x8001ecbc *)addr;
        h->x0 = 0;
        v = D_80085FB0[o->x64].x4;
        o->x7D = 1;
        o->x80 = id;
        o->x84 = a1;
        o->x7C = 0;
        o->x4 = 0;
        o->xC = a1;
        D_800AF5C0.x0 = (int)h;
        h->x8 = a1;
        *(V5_0x8001ecbc *)&o->x10 = *(V5_0x8001ecbc *)&h->xC;
        h->x20 = 0x6D6D7564;
        h->x4 = v;
        o->x8 = v;
        return 1;
    }
    return 0;
}
