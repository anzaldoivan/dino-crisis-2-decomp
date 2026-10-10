#include "common.h"

typedef struct Obj0x800371a0 {
    unsigned short state;
} Obj0x800371a0;
extern Obj0x800371a0 D_800B24A0;
extern void (*D_800893D8[])(Obj0x800371a0 *);

void func_800371A0(void)
{
    D_800893D8[D_800B24A0.state](&D_800B24A0);
}
