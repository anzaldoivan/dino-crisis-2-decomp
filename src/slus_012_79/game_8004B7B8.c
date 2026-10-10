#include "common.h"
#include "dc2.h"

typedef struct {
    int f0;
    char pad4[4];
    unsigned char f8[16];
    char pad18[0x20];
    unsigned short f38;
    char pad3A[0x13];
    unsigned char f4D;
    unsigned char f4E;
    unsigned char f4F;
    char pad50[8];
    unsigned char f58;
    char pad59[5];
    unsigned char f5E;
    unsigned char f5F;
    signed char f60;
    unsigned char f61;
    char pad62[2];
    unsigned char f64;
} Obj8004B7B8;
extern int func_80034CD8(int, int);
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ED9C(int);
extern void func_8004262C(int);
extern void func_80042758(int);

void func_8004B7B8(Obj8004B7B8 *a)
{
    unsigned int i;
    int s;

    a->f4D = 0;
    a->f4F = 0;
    a->f4E = 0;
    a->f58 = 0;
    a->f61 = 0;
    a->f5E = 0;
    a->f5F = 0;
    for (i = 0; i < 16; i++) {
        a->f8[i] = 0;
    }
    a->f0 = 1;
    if (func_80034CD8(2, 0x20)) {
        a->f64 = 5;
        return;
    }
    s = a->f64;
    if (s == 1) {
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        if (a->f38 != 0x1F) {
            func_8001ED9C(a->f38);
        }
    } else if (s == 4) {
        func_8004262C(4);
        WORK->x6E8 = s;
    } else if (s == 3) {
        func_80042758(0);
        func_8004262C(a->f60);
        WORK->x6E8 = a->f60;
    }
}
