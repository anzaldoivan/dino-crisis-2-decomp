#include "common.h"

typedef struct {
    unsigned short f0;
    unsigned short f2;
    unsigned short f4;
    unsigned short f6;
    unsigned char pad8;
    unsigned char n;
} Src0x800359fc;
typedef struct {
    unsigned char pad0;
    unsigned char kind;
    unsigned char idx;
    unsigned char pad3[9];
    short v[7];
} Obj0x800359fc;
extern Obj0x800359fc *func_800474DC(void);

void func_800359FC(Src0x800359fc *s)
{
    Obj0x800359fc *o;
    short *p;

    if (s->n == 0) {
        return;
    }
    o = func_800474DC();
    if (o == 0) {
        return;
    }
    o->kind = 4;
    o->idx = s->n - 1;
    p = o->v;
    p[0] = s->f0;
    p[1] = s->f2;
    p[2] = s->f4;
    p[3] = 0;
    p[4] = 0;
    p[5] = s->f6;
    p[6] = 0;
}
