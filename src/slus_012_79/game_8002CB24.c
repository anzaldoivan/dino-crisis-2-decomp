#include "common.h"

#include "dc2.h"
typedef Vec0x80033c70 Vec0x8002cb24;
typedef struct Obj0x8002cb24 {
    char pad0[0x40];
    Vec0x8002cb24 x40;
    char pad46[0x48 - 0x46];
    short x48;
    char pad4a[0x55 - 0x4A];
    unsigned char x55;
    char pad56[0x74 - 0x56];
    Vec0x8002cb24 x74;
    char pad7a[0x7C - 0x7A];
    Vec0x8002cb24 x7C;
    char pad82[0x9E - 0x82];
    short x9E;
} Obj0x8002cb24;
extern void (*D_80088F48[])(Obj0x8002cb24 *);

void func_8002CB24(Obj0x8002cb24 *p)
{
    *(int *)&p->x74 = *(int *)&p->x40;
    p->x74.z = p->x40.z;
    p->x40.x += p->x7C.x;
    p->x40.y += p->x7C.y;
    p->x40.z += p->x7C.z;
    p->x48 += p->x9E;
    D_80088F48[p->x55](p);
}
