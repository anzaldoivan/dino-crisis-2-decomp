#include "common.h"

typedef struct Key0x8002d0a4 {
    int v;
    unsigned char pad4[0x28 - 0x4];
} Key0x8002d0a4;
typedef struct Anim0x8002d0a4 {
    Key0x8002d0a4 *cur;
    unsigned char idx;
    unsigned char pad5;
    unsigned char frame;
    unsigned char start;
    unsigned char pad8[0xC - 0x8];
    int flags;
} Anim0x8002d0a4;
typedef struct Obj0x8002d0a4 {
    unsigned char pad0[0x54];
    unsigned char mode;
    unsigned char pad55[0x98 - 0x55];
    Anim0x8002d0a4 anim;
} Obj0x8002d0a4;
extern void (*D_80088F60[])(Obj0x8002d0a4 *);

void func_8002D0A4(Obj0x8002d0a4 *p)
{
    Anim0x8002d0a4 *a;
    Key0x8002d0a4 *k;
    int v;

    a = &p->anim;
    D_80088F60[p->mode](p);
    k = p->anim.cur;
    v = k->v;
    if (v == -1) {
        if (a->flags & 8) {
            p->anim.cur = k - (a->start - a->idx);
            a->frame = v;
            a->start = a->idx;
        } else {
            p->mode = 3;
        }
    }
}
