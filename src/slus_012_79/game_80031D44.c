#include "common.h"

#include "dc2.h"
typedef struct Seg0x80031d44 {
    SVECTOR v;
    char pad8[2];
    unsigned char idx;
    unsigned char flags;
} Seg0x80031d44;
extern int D_800AF11C;
char *func_800474B4(int a, int b);
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007A824(SVECTOR *r, MATRIX *m);
extern int func_800337D8();
extern int func_8004A0F8();
extern int func_8004A144();

int func_80031D44(char *obj)
{
    SVECTOR a;
    SVECTOR v;
    MATRIX m;
    unsigned int i;
    int found;
    unsigned int best;
    unsigned int d;
    Unit *u;
    Seg0x80031d44 *s;
    char *b;

    found = 0;
    best = -1;
    a.vx = *(short *)(obj + 0x74);
    a.vy = *(short *)(obj + 0x78);
    a.vz = *(short *)(obj + 0x40);
    a.pad = *(short *)(obj + 0x44);
    for (i = 1; i < D_800AF11C; i++) {
        u = &WORK->units[i];
        if (*(Seg0x80031d44 **)((char *)u + 0x12C) == 0) continue;
        func_8007A824((SVECTOR *)((char *)u + 0x48), &m);
        s = *(Seg0x80031d44 **)((char *)u + 0x12C);
        s--;
        do {
            s++;
            if (s->idx == 0xFF) {
                func_80079704(&m, &s->v, &v);
                v.vx += u->x40;
                v.vy += u->x42;
                v.vz += u->x44;
            } else {
                b = func_800474B4(u->id, s->idx);
                func_80079704((MATRIX *)(b + 0x20), &s->v, &v);
                v.vx += *(short *)(b + 0x40);
                v.vy += *(short *)(b + 0x42);
                v.vz += *(short *)(b + 0x44);
            }
            v.pad = s->v.pad;
            if (func_800337D8(&a, &v)) {
                found = 1;
                d = func_8004A144(func_8004A0F8(&v, obj + 0x74));
                if (d < best) best = d;
            }
        } while (s->flags & 1);
    }
    if (found) found = best;
    return found;
}
