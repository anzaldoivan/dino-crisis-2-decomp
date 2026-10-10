#include "common.h"

extern int *D_800AB9F8;
void func_800812E4(int arg0, int *arg1);

void func_8001CDE4(int arg0)
{
    int *p = D_800AB9F8;
    p[0] = 2;
    p[2] = arg0;
    func_800812E4(0xFF000000, p);
}
