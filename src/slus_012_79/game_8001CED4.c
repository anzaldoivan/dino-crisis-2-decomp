#include "common.h"

typedef struct Obj0x8001ced4 {
    int f0;
    int f4;
    int pad8[2];
    int f10;
} Obj0x8001ced4;
extern Obj0x8001ced4 *D_800AB9F8;
extern void func_800812D4(int arg0);
extern void func_800812E4();
extern int func_80081324(void);
extern void func_80081334(void);

void func_8001CED4(int arg0)
{
    D_800AB9F8->f0 = 1;
    D_800AB9F8->f4 = arg0;
    func_80081324();
    func_800812D4(D_800AB9F8->f10);
    func_80081334();
    func_800812E4(0xFF000000);
}
