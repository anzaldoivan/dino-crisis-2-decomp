#include "common.h"

typedef struct Obj0x8001ce18 {
    int f0;
    int pad4[3];
    int f10;
} Obj0x8001ce18;
extern Obj0x8001ce18 *D_800AB9F8;
extern void func_800812D4(int arg0);
extern void func_800812E4();
extern int func_80081324(void);
extern void func_80081334(void);

void func_8001CE18(void)
{
    D_800AB9F8->f0 = 0;
    func_80081324();
    func_800812D4(D_800AB9F8->f10);
    func_80081334();
    func_800812E4(0xFF000000);
}
