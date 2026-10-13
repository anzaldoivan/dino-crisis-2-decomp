#include "common.h"
#include "dc2.h"

typedef struct Rec0x80035900 {
    unsigned char unit;
    unsigned char seq;
    unsigned char step;
    unsigned char timer;
} Rec0x80035900;
extern unsigned char *D_800892DC[];
extern void func_80044834(void *, int);

void func_80035900(Rec0x80035900 *p)
{
    unsigned int i;
    unsigned char *s;

    for (i = 0; i < 4; i++, p++) {
        if (p->unit != 0) {
            if (--p->timer == 0xFF) {
                s = D_800892DC[p->seq] + p->step * 2;
                func_80044834(&WORK->units[p->unit], s[0]);
                if (s[1] != 0) {
                    p->step++;
                    p->timer = s[1];
                } else {
                    p->unit = 0;
                }
            }
        }
    }
}
