#include "common.h"

typedef struct Obj0x8002b844 {
    char pad0[0x4C];
    unsigned short f4C;
    char pad4E[0x9E - 0x4E];
    unsigned short f9E;
} Obj0x8002b844;
void func_8002B810(Obj0x8002b844 *arg0);

void func_8002B844(Obj0x8002b844 *arg0)
{
    arg0->f4C += arg0->f9E;
    func_8002B810(arg0);
}
