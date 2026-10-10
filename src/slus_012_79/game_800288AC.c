#include "common.h"

typedef struct {
    char pad0[0x2474];
    unsigned char x2474;
} Obj0x800288ac;
extern Obj0x800288ac D_800AFF68;
extern void (*D_80088D9C[])(Obj0x800288ac *);
extern void func_80018020(void *, int);
extern void func_8001CDE4(int);
extern void func_8005AD28();

void func_800288AC(void)
{
    Obj0x800288ac *p = &D_800AFF68;

    func_80018020(p, 0x928);
    while (1) {
        D_80088D9C[p->x2474](p);
        func_8005AD28();
        func_8001CDE4(1);
    }
}
