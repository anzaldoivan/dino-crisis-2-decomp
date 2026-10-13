#include "common.h"
#include "dc2.h"

typedef struct {
    unsigned short v[4];
} Map0x8001c72c;
extern Map0x8001c72c D_800181E8;

void func_8001C72C(unsigned short *in, unsigned short *out)
{
    unsigned short v;
    unsigned short bits[4];
    unsigned short i;
    Map0x8001c72c map;
    unsigned short old;


    v = *in & 0xFF0F;
    map = D_800181E8;
    switch (WORK->x108B) {
    case 1:
        bits[0] = 0x10;
        bits[1] = 0x40;
        bits[2] = 0x80;
        bits[3] = 0x20;
        break;
    case 0:
        bits[0] = 0x20;
        bits[1] = 0x10;
        bits[2] = 0x40;
        bits[3] = 0x80;
        break;
    case 2:
        bits[0] = 0x40;
        bits[1] = 0x10;
        bits[2] = 0x80;
        bits[3] = 0x20;
        break;
    }
    old = out[0];
    for (i = 0; i < 4; i++) {
        if (*in & bits[i]) {
            v |= map.v[i];
        }
    }
    out[2] = old;
    out[0] = v;
    out[1] = v & ~old;
}
