#include "common.h"

typedef struct {
    char pad0[0x1C0];
    int f1C0;
} Obj0x8002b350;
extern int func_800487B8(Obj0x8002b350 *);

void func_8002B350(Obj0x8002b350 *arg0)
{
    arg0->f1C0 = func_800487B8(arg0);
}
