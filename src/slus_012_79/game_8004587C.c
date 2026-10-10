#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned char pad[0x30];
    unsigned short unk30;
    unsigned short unk32;
} Obj8004587C;
extern Obj8004587C D_800B32F0;
extern unsigned short *func_800458F0(int a, int b);

int func_8004587C(void) {
    Work *s = WORK;
    Obj8004587C *o = &D_800B32F0;
    unsigned short v = *func_800458F0(s->xC2D, s->xC2C);
    unsigned short cur;
    if (v != 0xFFFF) {
        cur = o->unk32;
        if (cur != v) {
            if ((cur & 0xFF00) == (v & 0xFF00)) {
                v |= 8;
            }
            o->unk30 = cur;
            o->unk32 = v;
        }
    }
    return v;
}
