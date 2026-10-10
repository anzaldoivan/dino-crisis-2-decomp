#include "common.h"

typedef struct {
    unsigned char x00[0x2D];
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char x30[0x60];
} Ent0x8001b538;
extern Ent0x8001b538 D_800ABF50[];

void func_8001B538(int color) {
    unsigned char g = (color & 0xFF00) >> 8;
    unsigned char b;
    D_800ABF50[1].r = color;
    D_800ABF50[0].r = color;
    b = ((unsigned int)color >> 16) & 0xFF;
    D_800ABF50[1].g = g;
    D_800ABF50[0].g = g;
    D_800ABF50[1].b = b;
    D_800ABF50[0].b = b;
}
