#include "common.h"

typedef struct {
    unsigned char pad[0x30];
    unsigned short unk30;
    unsigned short unk32;
} Obj;
extern Obj D_800B32F0;
extern unsigned short *func_800458F0(int a, int b);

int func_8004587C(void) {
    unsigned char *s = *(unsigned char **)0x1F800000;
    Obj *o = &D_800B32F0;
    unsigned short v = *func_800458F0(s[0xC2D], s[0xC2C]);
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
