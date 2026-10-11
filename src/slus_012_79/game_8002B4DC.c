#include "common.h"
#include "dc2.h"

#define M2C_UNK s32
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
M2C_UNK func_80079704(void *, void *, void *);

void func_8002B4DC(void *arg0, void *arg1) {
    if (M2C_FIELD(arg0, s32 *, 0x60) != 0) {
        func_80079704(arg0 + 0x20, arg0 + 0x40, arg1);
        M2C_FIELD(arg1, u16 *, 0) = (u16) (M2C_FIELD(arg1, u16 *, 0) + M2C_FIELD(arg0, u16 *, 0x34));
        M2C_FIELD(arg1, u16 *, 2) = (u16) (M2C_FIELD(arg1, u16 *, 2) + M2C_FIELD(arg0, u16 *, 0x38));
        M2C_FIELD(arg1, u16 *, 4) = (u16) (M2C_FIELD(arg1, u16 *, 4) + M2C_FIELD(arg0, u16 *, 0x3C));
    }
}
