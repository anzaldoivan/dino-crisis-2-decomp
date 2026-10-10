#include "common.h"

typedef struct Obj_s8001E408 {
    unsigned char pad0[0x6A];
    unsigned char b6a;
    unsigned char b6b;
    unsigned char step;
    unsigned char timer;
} Obj_s8001E408;
extern int func_80078324(void);
extern void func_800782E4(void);

void func_8001E408(Obj_s8001E408 *p)
{
    switch (p->step) {
    case 0:
        if (func_80078324() == 1) {
            goto done;
        }
        p->timer = 30;
        p->step++;
        return;
    case 1:
        if (--p->timer == 0xFF) {
            func_800782E4();
        done:
            p->step = 0;
            p->b6a = p->b6b;
        }
        return;
    }
}
