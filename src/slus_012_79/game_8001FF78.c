#include "common.h"
#include "dc2.h"

typedef struct {
    short x, y, w, h;
} Rect0x8001ff78;
/* Arg0x8001ff78: dc2.h (T8 w37) */
extern char *D_80087564[];
extern int func_8007B380(int);
extern int func_8007B62C(Rect0x8001ff78 *, char *);

void func_8001FF78(Obj0x8002002c *obj, Arg0x8001ff78 *arg)
{
    Rect0x8001ff78 r;

    r.x = arg->x8;
    r.y = obj->x70 + arg->xa;
    r.w = arg->xc;
    r.h = arg->xe;
    if (r.w == 0x100) {
        if (arg->xe - obj->x70 < 5) {
            r.h = arg->xe - obj->x70;
        } else {
            r.h = 4;
        }
        obj->x70 += 4;
    }
    func_8007B62C(&r, D_80087564[obj->x69]);
    func_8007B380(0);
}
