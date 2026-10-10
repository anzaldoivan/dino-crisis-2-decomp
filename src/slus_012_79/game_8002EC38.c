#include "dc2.h"

typedef struct {
    unsigned int tag;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char code;
    short x;
    short y;
    short w;
    short h;
} Tile0x8002ec38;
typedef struct {
    short x;
    short y;
    short w;
    short h;
    int step;
    union {
        int v;
        struct {
            unsigned short lo;
            unsigned short hi;
        } h;
    } val;
    unsigned char flag;
    unsigned char cnt;
    unsigned char mode;
} Obj0x8002ec38;
extern unsigned int *D_800ABA98;
extern void func_80034C48();
extern int func_80034CD8();
extern int func_8007E034();
extern void func_8007E114();
extern void func_8007E194();
#define T0x8002ec38 ((Tile0x8002ec38 *)PKT)

void func_8002EC38(Obj0x8002ec38 *p)
{
    int mode;
    int c;
    unsigned int *ot;
    int tp;

    mode = p->mode;
    if (!func_80034CD8(2, 0x1D) && !func_80034CD8(2, 0x16)) {
        if ((unsigned char)--p->cnt == 0xFF) {
            func_80034C48(2, 6, 0);
        }
        switch (p->flag & 1) {
        case 0:
            p->val.v -= p->step;
            if (!func_80034CD8(2, 6)) {
                p->mode = 0;
                p->val.v = 0;
            }
            break;
        case 1:
            if (func_80034CD8(2, 6)) {
                p->val.v += p->step;
            } else {
                p->val.v = 0xFF0000;
            }
            break;
        }
    }
    c = p->val.h.hi;
    if (mode == 3) {
        T0x8002ec38->r = c;
        T0x8002ec38->g = T0x8002ec38->b = 0;
    } else {
        T0x8002ec38->r = T0x8002ec38->g = T0x8002ec38->b = c;
    }
    func_8007E114(PKT);
    T0x8002ec38->code |= 2;
    T0x8002ec38->x = p->x;
    T0x8002ec38->y = p->y;
    T0x8002ec38->w = p->w;
    T0x8002ec38->h = p->h;
    ot = &D_800ABA98[30];
    *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT += 4;
    if (mode == 1) {
        tp = func_8007E034(0, 2, 0x140, 0x100);
        func_8007E194(PKT, 1, 0, tp, 0);
    } else if (mode != 0 && mode < 4) {
        tp = func_8007E034(0, 1, 0x140, 0x100);
        func_8007E194(PKT, 1, 0, tp, 0);
    }
    ot = &D_800ABA98[30];
    *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT += 3;
    if (mode == 3) {
        c = p->val.h.hi;
        T0x8002ec38->r = T0x8002ec38->g = T0x8002ec38->b = c;
        func_8007E114(PKT);
        T0x8002ec38->code |= 2;
        T0x8002ec38->x = p->x;
        T0x8002ec38->y = p->y;
        T0x8002ec38->w = p->w;
        T0x8002ec38->h = p->h;
        ot = &D_800ABA98[30];
        *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
        *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
        PKT += 4;
        tp = func_8007E034(0, 2, 0x140, 0x100);
        func_8007E194(PKT, 1, 0, tp, 0);
        ot = &D_800ABA98[30];
        *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
        *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
        PKT += 3;
    }
}
