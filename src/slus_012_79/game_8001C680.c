#include "common.h"

typedef struct {
    unsigned char *base;
    unsigned char *data;
    unsigned char *end;
    unsigned int f0 : 1;
    unsigned int f1 : 1;
    unsigned int f2 : 4;
    unsigned int f6 : 1;
    unsigned int f7 : 1;
} Obj0x8001c680;
extern Obj0x8001c680 *func_8001C478();
extern Obj0x8001c680 *func_8001C510();

Obj0x8001c680 *func_8001C680(int sel, unsigned char *b)
{
    Obj0x8001c680 *p;

    if (sel) {
        p = func_8001C510();
    } else {
        p = func_8001C478();
    }
    if (p) {
        p->base = b;
        p->data = b + 4;
        p->f0 = 1;
        p->f1 = 0;
        p->end = b + b[0];
        p->f6 = b[1] != 0;
        p->f7 = b[2] != 0;
    }
    return p;
}
