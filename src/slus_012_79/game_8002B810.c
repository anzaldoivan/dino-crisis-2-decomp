#include "common.h"
#include "dc2.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void func_8002B810(void *arg0) {
    M2C_FIELD(arg0, u16 *, 0x7C) = (u16) (M2C_FIELD(arg0, u16 *, 0x7C) + M2C_FIELD(arg0, u16 *, 0x98));
    M2C_FIELD(arg0, u16 *, 0x7E) = (u16) (M2C_FIELD(arg0, u16 *, 0x7E) + M2C_FIELD(arg0, u16 *, 0x9A));
    M2C_FIELD(arg0, u16 *, 0x80) = (u16) (M2C_FIELD(arg0, u16 *, 0x80) + M2C_FIELD(arg0, u16 *, 0x9C));
}
