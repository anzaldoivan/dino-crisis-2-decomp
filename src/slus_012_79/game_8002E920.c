#include "common.h"

typedef struct {
    char pad0[0x40];
    short x;                    
    short y;                    
    short z;                    
    char pad46[0x54 - 0x46];
    unsigned char state;           
    char pad55[0x7C - 0x55];
    short vx;                   
    short vy;                   
    short vz;                   
    char pad82[0x84 - 0x82];
    unsigned char flags;           
} Obj8002E920;
int func_80048274(Obj8002E920 *o);

void func_8002E920(Obj8002E920 *o)
{
    o->x += o->vx;
    o->y += o->vy;
    o->z += o->vz;
    if (func_80048274(o) != 0 && !(o->flags & 8)) {
        o->state++;
    }
}
