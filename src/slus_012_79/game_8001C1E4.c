#include "common.h"

typedef struct {
    unsigned char pad0[0xC];
    int flags;
} Obj0x8001c1e4;
extern void func_8001BF5C(Obj0x8001c1e4 *);
extern void func_8001C050(Obj0x8001c1e4 *);

void func_8001C1E4(Obj0x8001c1e4 *p)
{
    if (p->flags & 0x40) {
        func_8001BF5C(p);
    }
    if (p->flags & 0x80) {
        func_8001C050(p);
    }
    if (!(p->flags & 0xC0)) {
        p->flags &= ~1;
    }
}
