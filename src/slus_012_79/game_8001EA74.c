#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x4C];
    int x4C;
    char pad50[0x64 - 0x50];
    unsigned short x64;
    short x66;
    char pad68[2];
    unsigned char x6A;
    char pad6b;
    unsigned char x6C;
    unsigned char x6D;
} Obj0x8001ea74;
extern Obj0x8001ea74 D_800AF5C0;
extern Ent0x8001ef60 D_80085FB0[];

int func_8001EA74(int a0)
{
    Obj0x8001ea74 *o = &D_800AF5C0;
    unsigned short id = a0;

    if (o->x6A != 0) {
        return 0;
    }
    o->x64 = id;
    o->x6A = 4;
    o->x6C = 0;
    o->x6D = 0;
    o->x66 = 0;
    o->x4C = D_80085FB0[id].x0;
    return 1;
}
