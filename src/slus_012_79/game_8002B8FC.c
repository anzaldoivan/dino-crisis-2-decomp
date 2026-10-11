#include "common.h"

typedef struct {
    short pad0[3];
    unsigned short v6;
} Sub0x8002b8fc;
typedef struct {
    unsigned char pad0;
    unsigned char b1;
    unsigned char pad2[0x16];
    unsigned char b18;
} Tgt0x8002b8fc;
typedef struct {
    unsigned char pad0[0x6C];
    unsigned short a6c;
    unsigned short a6e;
    unsigned char pad70[0x1C];
    Tgt0x8002b8fc *p8c;
    unsigned char pad90[0x8];
    Sub0x8002b8fc s98;
} Obj0x8002b8fc;
extern void func_80047814();
extern void func_8002B810();

void func_8002B8FC(Obj0x8002b8fc *arg0)
{
    Sub0x8002b8fc *s = &arg0->s98;
    Tgt0x8002b8fc *t;

    t = arg0->p8c;
    arg0->a6e += s->v6;
    arg0->a6c += s->v6;
    if ((short)arg0->a6e < 0) {
        arg0->a6e = 0;
    } else if ((short)arg0->a6c < 0) {
        arg0->a6c = 0;
    } else {
        goto end;
    }
    func_80047814(arg0);
    if (t != 0 && t->b1 == 1) {
        t->b18--;
    }
end:
    if (arg0->a6e >= 0x100) {
        arg0->a6e = 0xFF;
    }
    if (arg0->a6c >= 0x100) {
        arg0->a6c = 0xFF;
    }
    func_8002B810(arg0);
}
