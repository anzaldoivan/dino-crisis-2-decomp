#include "dc2.h"

typedef struct Obj0x8004c960 {
    char pad0[0x5E];
    unsigned char x5E;
    unsigned char x5F;
    char pad60[0x80 - 0x60];
    unsigned short x80;
    unsigned short x82;
} Obj0x8004c960;
typedef struct { char *s[9]; } Tbl0x8004c960;
typedef struct { unsigned char b[2][16]; } Blk32_0x8004c960;
extern Tbl0x8004c960 D_80019CAC;
extern Name8 D_80019CD0;
extern Blk32_0x8004c960 D_80019CD8;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC200[][0x400];
extern void func_8004CE64(int a, int b, int c, int d);
extern void func_800501C4(void *p, int a, int b, int c, int d, int e, int f);
extern void func_80050398(int a, int b, int c, int d, int e, int f, int g);
extern int func_80050A90(char *s, int x, int y, int z, int a, int b);
extern void func_80050C90(void *p, int a, int b, int c, int d, int e, int f);
extern void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
extern void func_8007E194(unsigned int *p, int a, int b, int c, int d);

void func_8004C960(Obj0x8004c960 *obj)
{
    func_80050398(0x18, 0x18, 0x26, 0x18, 0x808080, 0, 100);
    func_80050398(0x18, 0x57, 0x26, 0x57, 0x808080, 0, 100);
    func_80050398(0x18, 0x18, 0x18, 0x57, 0x808080, 0, 100);
    func_80050398(0x69, 0x18, 0x78, 0x18, 0x808080, 0, 100);
    func_80050398(0x69, 0x57, 0x78, 0x57, 0x808080, 0, 100);
    func_80050398(0x78, 0x18, 0x78, 0x57, 0x808080, 0, 100);

    if (WORK->x4 & 1) {
        obj->x80++;
    }
    obj->x80 &= 7;
    func_8004CE64(0x1B, 0x1C, obj->x80, 1);

    if (WORK->x4 & 1) {
        obj->x82--;
    }
    obj->x82 &= 7;
    func_8004CE64(0x74, 0x1C, obj->x82, -1);

    if (WORK->xC2B == 1) {
        func_80050D80(D_80185038, 0x48, 0x17, 0xC, 0x7C8F, 0x707070, 2);
    } else if (WORK->xC2B == 0) {
        func_80050D80(D_80185030, 0x48, 0x17, 0xC, 0x7C8F, 0x707070, 2);
    }

    {
        Tbl0x8004c960 tbl = D_80019CAC;

        if (!(WORK->x4 & 3)) {
            obj->x5E++;
        }
        if (func_80050A90(tbl.s[obj->x5F], 0x50, 0x28, obj->x5E, 0x505050, 100)) {
            obj->x5E = 0;
            if (++obj->x5F >= 9) {
                obj->x5F = 0;
            }
        }
    }
    {
        Name8 b = D_80019CD0;

        func_80050C90(&b, 0x40, 0, 0x7CC0, 0x808080, 0, 100);
    }

    func_8007E194(PKT, 1, 0, 0x8D, 0);
    ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)D_800AC200[D_800ABA96])->addr;
    ((Tag0x8004c960 *)D_800AC200[D_800ABA96])->addr = (unsigned int)PKT;
    PKT += 3;

    {
        Blk32_0x8004c960 c = D_80019CD8;

        func_800501C4(c.b[0], 0x1000, 0x1000, 0x8000, 0x8000, 1, 200);
        func_800501C4(c.b[1], 0x8000, 0x8000, 0x1000, 0x1000, 1, 200);
    }

    func_80050D80(D_801836C8, 0x48, 0x18, 0xD, 0x7C8E, 0x80808080, 2);
}
