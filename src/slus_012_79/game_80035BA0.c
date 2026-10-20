#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x13];
    unsigned char x13;
    char pad14[0x20 - 0x14];
} Elem0x80035ba0;
typedef struct {
    short x;
    short y;
    short z;
} Pos0x80035ba0;
char *func_800474B4(int a, int b);
extern int func_8004A378(void *, void *);

int func_80035BA0(Elem0x80035ba0 *e, int n)
{
    Unit *u;
    Unit *t;
    Pos0x80035ba0 p;
    unsigned char i;

    u = WORK->x6F0;
    if (u == 0) {
        u = WORK->units;
    }
    if (u->flags & 0x100) {
        t = (Unit *)func_800474B4(u->id, 8);
        p.x = t->x40;
        p.y = t->x42;
        p.z = t->x44;
    } else {
        p.x = u->x40;
        p.y = u->x42;
        p.z = u->x44;
    }
    for (i = 0; i < n; e++, i++) {
        if (e->x13 == u->x5A && func_8004A378(&p, e)) {
            // !FAKE: shared exit with the 0xFF fallthrough; return in-loop drifts length (run 2)
            goto end;
        }
    }
    i = 0xFF;
end:
    return i;
}
