#include "common.h"
#include "dc2.h"

typedef struct Def0x80035814 {
    unsigned char x0;
    unsigned char x1;
} Def0x80035814;
extern Def0x80035814 *D_800892DC[];
void func_80044834(void *, int);

void func_80035814(unsigned char arg0, unsigned char arg1) {
    Slot0x80035814 *p = &WORK->x658;
    Def0x80035814 *d;
    unsigned int i;

    if (arg0 == 0) {
        return;
    }
    arg0--;
    if (arg0 >= 6) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (p->x0 == 0) {
            p->x1 = arg0;
            p->x2 = 0;
            d = D_800892DC[p->x1];
            func_80044834(WORK->units + p->x0, d->x0);
            if (d->x1 != 0) {
                p->x0 = arg1;
                p->x2++;
                p->x3 = d->x1;
            }
            return;
        }
    }
}
