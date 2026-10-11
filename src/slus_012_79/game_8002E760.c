#include "common.h"
#include "dc2.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void func_8002E760(void *arg0) {
    M2C_FIELD(arg0, s16 *, 0x40) = 0xA3;
    M2C_FIELD(arg0, s16 *, 0x42) = 0x7C;
    M2C_FIELD(arg0, s16 *, 0x44) = 1;
}
