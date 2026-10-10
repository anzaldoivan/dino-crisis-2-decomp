#include "common.h"

typedef struct {
    unsigned char pad0[0xC];
    unsigned int flags;
    unsigned char pad10[0x3];
    unsigned char unk13;
} Obj8001C5AC;
extern Obj8001C5AC *func_8001C478();
extern Obj8001C5AC *func_8001C510();

Obj8001C5AC *func_8001C5AC(int a, int b) {
    Obj8001C5AC *p;
    if (a) {
        p = func_8001C510();
    } else {
        p = func_8001C478();
    }
    if (p) {
        p->unk13 = 1;
        p->flags = (p->flags | 3) & ~4;
        ((unsigned char *)p)[0xE] = b;
    }
    return p;
}
