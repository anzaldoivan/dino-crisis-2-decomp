#include "common.h"
#include "dc2.h"

extern char *D_80087564[];
extern int func_8007B380(int);
extern int func_8007B62C(Rect0x8006c2d8 *, char *);

void func_8001FEA8(Obj0x8002002c *obj, Arg0x8001ff78 *arg)
{
    Rect0x8006c2d8 r;

    r.w = 0x20;
    r.x = obj->x72 + arg->x8;
    r.y = obj->x70 + arg->xa;
    r.h = arg->xe - obj->x70;
    if (r.h > 0x20) {
        r.h = 0x20;
    }
    func_8007B62C(&r, D_80087564[obj->x69]);
    obj->x72 += 0x20;
    if (obj->x72 >= arg->xc) {
        obj->x72 = 0;
        obj->x70 += 0x20;
    }
    func_8007B380(0);
}
