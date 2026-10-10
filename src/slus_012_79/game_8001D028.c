#include "common.h"
#include "dc2.h"

extern Ent0x8001cf84 jtbl_800181F0[];
extern void func_8001CDAC(int, int);
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);

void func_8001D028(int a0, int idx)
{
    Ent0x8001cf84 *e;

    e = &jtbl_800181F0[idx];
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001ECBC(e->id, *e->data);
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001CDAC(a0, e->arg);
}
