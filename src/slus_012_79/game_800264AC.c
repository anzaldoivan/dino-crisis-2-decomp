#include "common.h"
#include "dc2.h"

extern u8 D_80087CEC[];
extern u8 func_8002BDE4(int);
extern void func_80026504(void *, void *);
typedef struct {
    u8 pad[8];
    u8 state;
} Sub;

void func_800264AC(u8 *obj) {
    Sub *sub = (Sub *)(obj + 0xC);
    u8 v;

    v = func_8002BDE4(0);
    sub->state = v;
    if ((u8)(v - 1) < 2) {
        func_80026504(obj, D_80087CEC);
    }
}
