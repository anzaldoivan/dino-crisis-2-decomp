#include "common.h"

typedef struct {
    unsigned char pad0[0x1C];
    unsigned char r0, g0, b0, p0;
    unsigned char r1, g1, b1, p1;
    unsigned char r2, g2, b2, p2;
} A28;
typedef struct {
    unsigned char pad0[0x24];
    unsigned char r0, g0, b0, p0;
    unsigned char r1, g1, b1, p1;
    unsigned char r2, g2, b2, p2;
    unsigned char r3, g3, b3, p3;
} A34;
typedef struct {
    A28 *a;
    A34 *b;
    unsigned short na;
    unsigned short nb;
} Hdr;
typedef struct {
    char pad[0x68];
    Hdr *hdr;
} Obj;

void func_800661FC(Obj *obj, unsigned char c)
{
    Hdr *h = obj->hdr;
    A28 *a = h->a;
    A34 *b = h->b;
    unsigned int i;

    for (i = 0; i < h->na; i++, a++) {
        a->b2 = c;
        a->g2 = c;
        a->r2 = c;
        a->b1 = c;
        a->g1 = c;
        a->r1 = c;
        a->b0 = c;
        a->g0 = c;
        a->r0 = c;
    }
    for (i = 0; i < h->nb; i++, b++) {
        b->b3 = c;
        b->g3 = c;
        b->r3 = c;
        b->b2 = c;
        b->g2 = c;
        b->r2 = c;
        b->b1 = c;
        b->g1 = c;
        b->r1 = c;
        b->b0 = c;
        b->g0 = c;
        b->r0 = c;
    }
}
