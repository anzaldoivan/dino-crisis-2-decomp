#include "common.h"

#include "dc2.h"
extern Obj0x8001cc74 D_800ABA08[4];
extern int func_80081324(void);
extern void func_800812D4(int arg0);
extern void func_80081334(void);

void func_8001CE68(int i)
{
    if (D_800ABA08[i].f0 != 0) {
        D_800ABA08[i].f0 = 0;
        func_80081324();
        func_800812D4(D_800ABA08[i].f10);
        func_80081334();
    }
}
