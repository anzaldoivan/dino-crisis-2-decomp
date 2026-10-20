#include "common.h"

typedef struct Obj0x80020188 {
    unsigned char pad0[0x69];
    unsigned char idx;
    unsigned char pad6A[0x70 - 0x6A];
    unsigned short count;
} Obj0x80020188;
typedef struct Arg0x80020188 {
    int pad0;
    int len;
    int data;
} Arg0x80020188;
extern unsigned char D_800AF63E;
extern int D_80087564[];
extern int func_80070484(int a);
extern void func_80070514(int a, int b);
extern void func_800705A4(int a);

void func_80020188(Obj0x80020188 *o, Arg0x80020188 *a)
{
    int c;

    D_800AF63E = 0;
    if (o->count == 0) {
        c = func_80070484(a->data);
        c = o->count;
        c++;
        o->count = c;
    }
    if (a->len > 0x800) {
        func_80070514(D_80087564[o->idx], 0x800);
    } else {
        func_80070514(D_80087564[o->idx], a->len);
    }
    func_800705A4(1);
}
