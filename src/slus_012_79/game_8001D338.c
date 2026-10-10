#include "common.h"
#include "dc2.h"

typedef struct Unit0x8001d338 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x50 - 0x46];
    int flags;
    char pad54[0x5A - 0x54];
    short x5A;
    char pad5c[0x64 - 0x5C];
    int id;
} Unit0x8001d338;
typedef struct Zone0x8001d338 {
    unsigned int x0;
    unsigned char x4;
    unsigned char x5;
    unsigned char x6;
    unsigned char x7;
    char pad8[4];
    char xC[0x2C - 0xC];
} Zone0x8001d338;
typedef struct Obj0x8001d338 {
    char pad0[0x4C];
    unsigned char x4C;
    unsigned char x4D;
    char pad4e[1];
    unsigned char x4F;
} Obj0x8001d338;
typedef struct WorkView0x8001d338 {
    char pad0[0x5E8];
    unsigned int x5E8;
} WorkView0x8001d338;
extern void func_8001DB64();
extern char *func_800474B4(int a, int b);
extern int func_8004A378(void *, void *);

void func_8001D338(Obj0x8001d338 *obj, Zone0x8001d338 *z)
{
    Unit0x8001d338 *u;
    Unit0x8001d338 *t;
    Pos0x8001d338 pos;
    unsigned short i;

    u = (Unit0x8001d338 *)WORK->units;
    if (u->flags & 0x100) {
        t = (Unit0x8001d338 *)func_800474B4(u->id, 8);
        pos.x = t->x40;
        pos.y = t->x42;
        pos.z = t->x44;
    } else {
        pos.x = u->x40;
        pos.y = u->x42;
        pos.z = u->x44;
    }
    for (i = 0; i < ((WorkView0x8001d338 *)WORK)->x5E8; z++, i++) {
        if (z->x6 <= u->x5A && u->x5A <= z->x6 + z->x7 && !(z->x0 & 0x80)
            && func_8004A378(&pos, z->xC) != 0) {
            obj->x4D = i;
            obj->x4F = z->x5;
            if (z->x4 != obj->x4C) {
                func_8001DB64(z->x4);
            }
            return;
        }
    }
}
