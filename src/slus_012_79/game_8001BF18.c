#include "common.h"
#include "dc2.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void func_8001BF18(s32 arg0) {
    s32 temp_v1;
    u32 temp_a0;
    u32 var_a1;

    var_a1 = arg0 + 0x190;
    temp_a0 = arg0 + 0x1C0;
    if (var_a1 < temp_a0) {
        do {
            temp_v1 = M2C_FIELD(var_a1, s32 *, 0xC);
            if (temp_v1 & 1) {
                M2C_FIELD(var_a1, s32 *, 0xC) = (s32) (temp_v1 & ~1);
            }
            var_a1 += 0x18;
        } while (var_a1 < temp_a0);
    }
}
