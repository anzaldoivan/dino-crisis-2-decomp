#include "common.h"
#include "dc2.h"

typedef struct {
    int f0;
    char pad4[0xB];
    unsigned char fF;
    char pad10[0x18];
} Rec0x8002d154;
typedef struct {
    Name8 head;
    char pad8[0x18];
    int f20, f24, f28, f2C, f30, f34, f38, f3C;
    short s40, s42, s44;
} Src0x8002d154;
typedef struct {
    Rec0x8002d154 *rec;
    unsigned char idx;
    unsigned char lim;
    signed char cur;
    unsigned char last;
    Src0x8002d154 *src;
    int flags;
} Seq0x8002d154;
typedef struct {
    char pad0[0x20];
    int f20, f24, f28, f2C, f30, f34, f38, f3C;
    Name8 f40;
    char pad48[0xC];
    unsigned char f54;
    char pad55[0x43];
    Seq0x8002d154 seq;
} Obj0x8002d154;
extern void func_8002D350();

void func_8002D154(Obj0x8002d154 *o)
{
    Seq0x8002d154 *s = &o->seq;
    Rec0x8002d154 *r;
    Src0x8002d154 *src;
    int t;
    int flags = s->flags;
    Rec0x8002d154 *rec = o->seq.rec;

    if (flags & 1) {
        s->cur = rec->fF;
    } else if (flags & 0x100) {
        t = s->idx;
        s->idx = 0;
        s->cur = t;
        if (s->rec->fF < s->cur) {
            do {
                r = s->rec;
                s->rec = r + 1;
                s->idx++;
                if (r[1].f0 == -1) {
                    o->f54 = 3;
                    return;
                }
            } while (s->rec->fF < (signed char)t);
        }
    } else {
        s->cur = 0;
        o->seq.rec += s->idx;
    }
    s->last = s->idx;
    if (s->idx >= s->lim) {
        s->lim = 0xFF;
    }
    if (s->flags & 0x20) {
        o->f40 = s->src->head;
        s->src = 0;
    } else if (s->flags & 0x200) {
        src = s->src;
        o->f20 = src->f20;
        o->f24 = src->f24;
        o->f28 = src->f28;
        o->f2C = src->f2C;
        o->f30 = src->f30;
        o->f34 = src->f34;
        o->f38 = src->f38;
        o->f3C = src->f3C;
        o->f34 = src->s40;
        o->f38 = src->s42;
        o->f3C = src->s44;
    }
    o->f54++;
    func_8002D350(o);
}
