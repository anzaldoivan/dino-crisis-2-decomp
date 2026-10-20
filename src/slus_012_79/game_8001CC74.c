#include "common.h"
#include "dc2.h"

extern Obj0x8001cc74 *D_800AB9F8;
extern Obj0x8001cc74 D_800ABA08[4];
extern int func_800812C4();
extern void func_800812E4();
extern int func_80081324(void);
extern void func_80081334(void);

void func_8001CC74(void)
{
    for (D_800AB9F8 = D_800ABA08; D_800AB9F8 < &D_800ABA08[4]; D_800AB9F8++) {
        switch (D_800AB9F8->f0) {
        case 1:
            func_80081324();
            D_800AB9F8->f10 = func_800812C4(D_800AB9F8->f4, D_800AB9F8->fC, 0);
            func_80081334();
            D_800AB9F8->f0 = 3;
            func_800812E4(D_800AB9F8->f10);
            break;
        case 2:
            if (--D_800AB9F8->f8 != 0) {
                break;
            }
        case 4:
            D_800AB9F8->f0 = 3;
            func_800812E4(D_800AB9F8->f10);
            break;
        }
    }
}
