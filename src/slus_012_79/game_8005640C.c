#include "common.h"
#include "dc2.h"

typedef struct Obj0x8005640c {
    union {
        int w;
        struct { char pad0[2]; unsigned char b2; } s;
    } x0;
    char pad4[0x60];
    unsigned char x64;
} Obj0x8005640c;
extern unsigned short D_800ABA90;
extern Blk8x0x80053db0 D_80019F30;
extern char D_8008A8BC[];
extern void func_80044834(void *, int);
extern void func_80050030();
extern int func_80050934();
int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
extern void func_8004D028();

void func_8005640C(Obj0x8005640c *obj)
{
    unsigned short pad = D_800ABA90;
    Blk8x0x80053db0 tmp;

    if (pad & 0x40) {
        func_80044834(0, 1);
        if (obj->x64 == 5) {
            obj->x0.w = 0x502;
            obj->x64 = 0;
        } else {
            obj->x0.s.b2 = 0xB;
        }
    } else if (pad & 0xA0) {
        func_80044834(0, 2);
        if (obj->x64 == 5) {
            obj->x0.w = 0x502;
            obj->x64 = 0;
        } else {
            obj->x0.s.b2 = 0xB;
        }
    }
    tmp = D_80019F30;
    func_80050030(&tmp, 0x404040, 3, 2);
    func_800506BC(D_8008A8BC, func_80050934(D_8008A8BC), 0x6C, 0x64, 0x808080, 1, 0);
    func_8004D028(obj, 0xA0, 0x60);
}
