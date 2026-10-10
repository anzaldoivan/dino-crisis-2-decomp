#include "common.h"
#include "dc2.h"

typedef struct {
    char pad[0x30];
    int x30;
} Obj0x80037694;
extern void func_8007B2E8(int);
extern void func_8007B380(int);
extern void func_8007B504(Box0x8002243c *, int, int, int);

void func_80037694(Obj0x80037694 *obj)
{
    Box0x8002243c r;
    func_8007B2E8(0);
    r.x = 0;
    r.y = 0;
    r.w = obj->x30 ? 0x1E0 : 0x140;
    r.h = 0x1F0;
    func_8007B504(&r, 0, 0, 0);
    func_8007B380(0);
    func_8007B2E8(1);
}
