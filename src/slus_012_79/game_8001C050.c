#include "common.h"

typedef struct {
    unsigned char *hdr;
    int pad4;
    unsigned char *p;
    union {
        int flags;
        struct {
            unsigned char pad[3];
            unsigned char count;
        } b;
    } f;
    short step;
    union {
        short val;
        struct {
            unsigned char lo;
            unsigned char hi;
        } b;
    } v;
    unsigned char pad14;
    unsigned char state;
    unsigned char pad16;
    unsigned char cnt;
} Fade0x8001c050;
extern unsigned char D_800A79BB;

void func_8001C050(Fade0x8001c050 *o)
{
    unsigned char *p = o->p;
    unsigned char *hdr = o->hdr;

    switch (o->state) {
    case 0:
        o->state = 1;
        o->cnt = hdr[2];
        o->f.b.count = p[0];
        if (o->f.b.count != 0) {
            o->step = (p[1] << 8) / o->f.b.count;
        } else {
            unsigned char t = p[1];
            o->v.b.lo = 0;
            o->v.b.hi = t;
        }
        o->v.val = 0;
    case 1:
        if (o->f.b.count == 0) {
            o->cnt--;
            p += 2;
            if (o->cnt == 0) {
                o->state++;
                return;
            }
            o->p = p;
            o->f.b.count = p[0];
            if (o->f.b.count != 0) {
                o->step = ((p[1] - p[-1]) << 8) / o->f.b.count;
            } else {
                o->step = 0;
                o->f.b.count = 1;
            }
        }
        o->f.b.count--;
        D_800A79BB = o->v.b.hi;
        if (o->f.b.count != 0) {
            o->v.val += o->step;
        } else {
            unsigned char t = p[1];
            o->v.b.lo = 0;
            o->v.b.hi = t;
        }
        return;
    case 2:
        o->f.flags &= ~0x80;
        return;
    }
}
