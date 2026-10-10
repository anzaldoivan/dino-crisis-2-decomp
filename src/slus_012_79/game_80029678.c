#include "dc2.h"

extern void func_8001CED4(int);
extern void func_80034C48(int, int, int);
extern void func_8002F398(void);

void func_80029678(void)
{
    func_80034C48(1, 3, 0);
    WORK->xF = 0;
    WORK->x204 = 2;
    WORK->x206 = 0;
    WORK->x208 = 0;
    WORK->x20A = 0;
    func_8001CED4((int)func_8002F398);
}
