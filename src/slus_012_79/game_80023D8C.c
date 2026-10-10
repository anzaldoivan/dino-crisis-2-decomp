#include "dc2.h"

typedef struct Sub0x80023d8c {
    char pad0[0xB4];
    int xB4;
    short xB8;
    short xBA;
    unsigned short xBC;
    char padBE[0xC4 - 0xBE];
    short xC4;
    char padC6[0xC8 - 0xC6];
    unsigned char xC8;
    unsigned char xC9;
    char padCA[0xCC - 0xCA];
    unsigned char xCC;
    char padCD;
    unsigned char xCE;
    unsigned char xCF;
} Sub0x80023d8c;
typedef struct Work0x80023d8c {
    char pad0[0x22C];
    Sub0x80023d8c x22C;
    char pad2dc[0x4E0 - 0x22C - sizeof(Sub0x80023d8c)];
    Unit0x80023d8c *units;
} Work0x80023d8c;
#define WORK0x80023d8c (*SCRATCH(Work0x80023d8c *, 0))
extern void func_800223BC(void);
extern void func_800205F4(void);

void func_80023D8C(void)
{
    Sub0x80023d8c *s = &WORK0x80023d8c->x22C;
    unsigned short v;

    func_800223BC();
    func_800205F4();
    v = WORK0x80023d8c->units->x108;
    s->xC8 = 1;
    s->xB8 = 0x3C;
    s->xC9 = 0;
    s->xBA = 0;
    s->xCC = 0;
    s->xC4 = 0;
    s->xB4 = 0;
    s->xCF = 0;
    s->xCE = 0;
    s->xBC = v;
}
