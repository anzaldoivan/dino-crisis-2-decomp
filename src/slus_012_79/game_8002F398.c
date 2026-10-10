#include "dc2.h"

extern void func_8001CDE4(int);
extern void (*D_80089200[])(void);

void func_8002F398(void)
{
    while (1) {
        D_80089200[WORK->x204]();
        func_8001CDE4(1);
    }
}
