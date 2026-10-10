#include "common.h"

typedef struct Sub0x8002cbe8 {
    unsigned char pad0[9];
    unsigned char b9;
    unsigned char bA;
    unsigned char bB;
    unsigned char bC;
} Sub0x8002cbe8;
typedef struct Obj0x8002cbe8 {
    unsigned char b0;
    unsigned char pad1[0x53];
    unsigned char b54;
    unsigned char pad55[0x98 - 0x55];
    Sub0x8002cbe8 sub;
} Obj0x8002cbe8;
extern void func_8002CC2C(Obj0x8002cbe8 *obj);

void func_8002CBE8(Obj0x8002cbe8 *obj)
{
    Sub0x8002cbe8 *s = &obj->sub;
    s->bA = 0;
    s->bC = 0;
    s->bB = s->b9;
    obj->b0 = 1;
    obj->b54++;
    func_8002CC2C(obj);
}
