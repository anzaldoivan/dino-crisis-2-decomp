#include "common.h"

#include "dc2.h"
typedef struct Obj8002C548 {
    unsigned char pad0[0x54];
    unsigned char mode;
    unsigned char pad55[0x7B - 0x55];
    unsigned char state;
    unsigned char pad7C[0x8C - 0x7C];
    unsigned char *flags;
} Obj8002C548;
extern void (*D_80088E10[])(Obj8002C548 *);
extern void (*D_80088F3C[])(Obj8002C548 *);
extern void func_8002B614(Obj8002C548 *o);
extern void func_80047814(Obj8002C548 *o);

void func_8002C548(Obj8002C548 *p)
{
    unsigned char *f;

    f = p->flags;
    D_80088E10[p->state](p);
    D_80088F3C[p->mode](p);
    func_8002B614(p);
    if (!(*f & 1)) {
        func_80047814(p);
    }
}
