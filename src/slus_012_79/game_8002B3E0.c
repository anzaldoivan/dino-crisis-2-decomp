#include "common.h"

typedef struct Obj8002B3E0 {
    unsigned char pad0[0x55];
    unsigned char state;
} Obj8002B3E0;
extern void (*D_80088E04[])(Obj8002B3E0 *);

void func_8002B3E0(Obj8002B3E0 *obj)
{
    D_80088E04[obj->state](obj);
}
