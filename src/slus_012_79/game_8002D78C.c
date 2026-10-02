#include "common.h"

typedef struct Obj {
    unsigned char pad0[0x46];
    unsigned char timer;                
    unsigned char pad47[0x54 - 0x47];
    unsigned char mode;                 
    unsigned char pad55[0x7B - 0x55];
    unsigned char state;                
} Obj;
extern void (*D_80088E10[])(Obj *);
extern void (*D_80089060[])(Obj *);
void func_8002B614(Obj *);
void func_8002B41C(Obj *);

void func_8002D78C(Obj *p)
{
    D_80088E10[p->state](p);
    D_80089060[p->mode](p);
    func_8002B614(p);
    if (--p->timer == 0) {
        func_8002B41C(p);
    }
}
