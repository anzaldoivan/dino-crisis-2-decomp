#include "common.h"

typedef struct Obj {
    char pad0[0x42];
    short x42;
    char pad44[0x50 - 0x44];
    int x50;
    union { int w; unsigned char b[4]; } x54;
    char pad58[0x5A - 0x58];
    short x5A;
    char pad5C[0xF0 - 0x5C];
    int xF0;
    short xF4;
    char padF6[0x108 - 0xF6];
    short x108;
    char pad10A[0x11B - 0x10A];
    unsigned char x11B;
    char pad11C[0x174 - 0x11C];
    unsigned short x174;
    unsigned short x176;
    char pad178[0x17B - 0x178];
    unsigned char x17B;
    char pad17C[0x1DC - 0x17C];
    int x1DC;
} Obj;
typedef struct {
    char pad0[0x28];
    short x28;
    char pad2A[0x58 - 0x2A];
    short x58;
    short x5A;
    char pad5C[0x60 - 0x5C];
    short x60;
    short x62;
} Cam;
typedef struct {
    char pad0[0xC2B];
    unsigned char xC2B;
} Scratch;
extern Cam *D_800B25DC;
int func_8003C9D0(int);
int func_8003D29C(void);
void func_8003EA64(Obj *);
void func_80044834(Obj *, int);
void func_800484F0(Obj *, int, int, int, int);
int func_800487B8(Obj *);

void func_80039654(Obj *obj)
{
    unsigned char side = obj->x54.b[1] - 2;

    switch (obj->x54.b[2]) {
    case 0:
        obj->xF0 = 0;
        if (side) {
            obj->xF4 = -0x80;
            func_800484F0(obj, 0xB, 0, 0, 4);
        } else {
            obj->xF4 = 0x80;
            func_800484F0(obj, 0xA, 0, 0, 4);
        }
        func_80044834(obj, 3);
        obj->x11B = 0;
        obj->x108 -= func_8003C9D0(obj->x176);
        obj->x174 = 1;
        obj->x50 &= ~0x20;
        obj->x54.b[2]++;
        D_800B25DC->x28 = (obj->x54.b[3] << 2) + 0x32;
        D_800B25DC->x60 = 0;
        D_800B25DC->x62 = 0;
        D_800B25DC->x58 = 0;
        D_800B25DC->x5A = 0;
        if (obj->x108 < 0) {
            obj->x54.w = 3;
        }
        break;
    case 1:
        if (func_800487B8(obj)) {
            if (side) {
                func_800484F0(obj, 0x11, 0, 1, 4);
            } else {
                func_800484F0(obj, 0x12, 0, 1, 4);
            }
            func_8003EA64(obj);
            obj->x174 |= 2;
            obj->xF4 = 0;
            obj->x54.b[2]++;
        }
        break;
    case 2:
        if ((*(Scratch **)0x1F800000)->xC2B == 2 && -(obj->x5A * 1000) != obj->x42) {
            break;
        }
        D_800B25DC->x28 = D_800B25DC->x28 - func_8003D29C() - 2;
        if (D_800B25DC->x28 <= 0) {
            obj->x174 &= ~2;
            func_8003EA64(obj);
            if (side) {
                func_800484F0(obj, 0xD, 0, 0, 4);
            } else {
                func_800484F0(obj, 0xC, 0, 0, 4);
            }
            obj->x54.b[2]++;
        } else {
            func_800487B8(obj);
        }
        break;
    case 3:
        if (func_800487B8(obj)) {
            func_8003EA64(obj);
            obj->x17B = 10;
            obj->x1DC = 8;
            obj->x174 = 0;
            obj->x54.w = 1;
            obj->x50 &= ~0x10;
            obj->x50 &= ~0x20;
            obj->x50 &= ~0x800;
            obj->x50 &= ~0x100;
        }
        break;
    }
}
