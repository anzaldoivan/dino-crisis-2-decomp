#include "common.h"

typedef struct Obj0x8002b614 {
    char pad0[0x20];
    int v20;
    int v24;
    int v28;
    int v2c;
    int v30;
    int v34;
    int v38;
    int v3c;
    short h40;
    short h42;
    short h44;
    char pad46[0x50 - 0x46];
    int flags50;
    char pad54[0x60 - 0x54];
    struct Obj0x8002b614 *link60;
} Obj0x8002b614;

void func_8002B614(Obj0x8002b614 *self)
{
    Obj0x8002b614 *p = self->link60;

    if (p != 0 && p != self) {
        if (p->flags50 & 1) {
            self->v20 = p->v20;
            self->v24 = p->v24;
            self->v28 = p->v28;
            self->v2c = p->v2c;
            self->v30 = p->v30;
            self->v34 = p->v34;
            self->v38 = p->v38;
            self->v3c = p->v3c;
            self->v34 = p->h40;
            self->v38 = p->h42;
            self->v3c = p->h44;
        } else {
            self->link60 = self;
        }
    }
}
