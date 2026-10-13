#include "common.h"
#include "dc2.h"

typedef struct {
    int x0;
    char pad4[0x4C - 0x4];
    int x4C;
    int x50;
    int x54;
    int x58;
    int x5C;
    int x60;
    unsigned short x64;
    short x66;
    char pad68[0x6A - 0x68];
    unsigned char x6A;
    char pad6b[0x6C - 0x6B];
    unsigned char x6C;
    unsigned char x6D;
    char pad6e[0x7C - 0x6E];
    unsigned char x7C;
    unsigned char x7D;
    char pad7e[0x80 - 0x7E];
    int x80;
    int x84;
    char pad88[0x94 - 0x88];
    int x94;
} Obj0x8001ec10;
extern Obj0x8001ec10 D_800AF5C0;
extern Ent0x8001ef60 D_80085FB0[];
extern int D_800AF128;

int func_8001EC10(int a0)
{
    unsigned short id = a0;
    Obj0x8001ec10 *o = &D_800AF5C0;

    if (o->x6A != 0) {
        return 0;
    }
    o->x6A = 1;
    o->x94 = 0;
    o->x7D = 0;
    o->x80 = id;
    o->x84 = 0;
    o->x64 = id;
    o->x6C = 0;
    o->x6D = 0;
    o->x66 = 0;
    D_800AF5C0.x0 = 0x801E6D00;
    D_800AF128++;
    o->x58 = D_80085FB0[id].x4;
    o->x60 = D_80085FB0[id].x4;
    o->x5C = D_80085FB0[id].x4;
    o->x4C = D_80085FB0[id].x0;
    o->x54 = D_80085FB0[id].x0;
    o->x50 = D_80085FB0[id].x0;
    o->x7C |= 1;
    return 1;
}
