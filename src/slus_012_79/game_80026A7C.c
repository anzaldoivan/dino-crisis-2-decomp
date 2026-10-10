#include "dc2.h"

typedef struct {
    char pad0[0x92];
    unsigned short x92;
    char pad94[0x280 - 0x94];
    char x280[0x28];
    char x2A8[8];
} Unit0x80026a7c;
typedef struct {
    char pad0[0x4E0];
    Unit0x80026a7c *x4E0;
    char pad4e4[0xC2B - 0x4E4];
    unsigned char xC2B;
} Work0x80026a7c;
typedef struct {
    char pad0[0x2477];
    unsigned char x2477;
    char pad2478[4];
    unsigned char x247C;
} Obj0x80026a7c;
#define WORK0x80026a7c (*SCRATCH(Work0x80026a7c *, 0))
extern int func_80034CD8(int, int);
extern void func_80044834(void *, int);
extern void func_800484F0();
extern void func_80079AA4();

void func_80026A7C(Obj0x80026a7c *p)
{
    Unit0x80026a7c *u = WORK0x80026a7c->x4E0;
    Unit0x80026a7c *v;
    Sub0x80026a7c *s;
    unsigned int c;

    switch (p->x2477) {
    case 1:
        if (func_80034CD8(2, 6) == 0) {
            c = WORK0x80026a7c->xC2B;
            if (c == 2) {
                p->x2477 = 3;
                p->x247C = c;
            } else {
                p->x2477++;
                func_800484F0(u, WORK0x80026a7c->xC2B == 0, 0, 0x18, 10);
            }
        }
        break;
    case 2:
        if (u->x92 < 10) {
            break;
        }
        p->x2477 = 3;
        func_80044834(0, 0x78);
    case 3:
        v = WORK0x80026a7c->x4E0;
        s = (Sub0x80026a7c *)((char *)v + 0x260);
        s->x4A += 0x12;
        func_80079AA4(v->x2A8, v->x280);
        c = u->x92;
        c = c < 0x1C;
        if (c == 0) {
            p->x2477 += 2;
        }
        break;
    }
}
