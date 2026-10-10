#include "dc2.h"

typedef struct {
    char pad[0x70];
} Shot0x80063468;
typedef struct {
    char pad[0xAC];
} Elem0x80063468;
typedef struct {
    char pad0[0x4DC];
    Shot0x80063468 *x4DC;
    void *units;
    Elem0x80063468 *x4E4;
} Work0x80063468;
#define WORK0x80063468 (*SCRATCH(Work0x80063468 *, 0))
typedef struct {
    int x0;
    char pad4[0x9E - 0x4];
    unsigned char x9E;
    signed char x9F;
} Obj0x80063468;
extern void func_80065F48(Shot0x80063468 *s);
extern void func_8002B41C(Elem0x80063468 *o);

int func_80063468(Obj0x80063468 *p)
{
    switch (p->x9E) {
    case 1:
        func_80065F48(&WORK0x80063468->x4DC[p->x9F + 12]);
        break;
    case 2:
        func_8002B41C(&WORK0x80063468->x4E4[p->x9F]);
        break;
    }
    p->x0 += 2;
    return 0;
}
