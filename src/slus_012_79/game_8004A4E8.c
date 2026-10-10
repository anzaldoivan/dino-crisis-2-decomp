#include "common.h"
#include "dc2.h"
#include "../shared/func_800436C8.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;









extern void func_80034C48(int, int, int);
extern void func_8001BF18(Unit *);


extern int func_8004A2C4(void *p, int arg);


extern int func_8004A2C4(void *p, int arg);
extern short func_8003DD34(int a, int b, int c);



extern int func_8007AC34(int, int);



void func_8004A4E8(unsigned char *a0)
{
    a0[0x118] &= 0x7F;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A4FC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A554);

typedef struct Obj0x8004a610 {
    char pad00[0x40];
    char sub40[0x10];
    int flags50;
    int busy54;
} Obj0x8004a610;
extern int func_8004A4FC();

void func_8004A610(Obj0x8004a610 *obj)
{
    if (obj->busy54 == 0) {
        obj->flags50 &= ~2;
        if (func_8004A4FC(obj->sub40) != 0) {
            obj->flags50 |= 2;
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A66C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A790);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A84C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A8C0);

typedef struct Obj0x8004a970 {
    char pad0[0x40];
    unsigned short f40;
    unsigned short f42;
    unsigned short f44;
    char pad46[0xE0 - 0x46];
    unsigned short fE0;
    unsigned short fE2;
    unsigned short fE4;
} Obj0x8004a970;
extern void func_8004A9A4();

void func_8004A970(Obj0x8004a970 *arg0)
{
    arg0->fE0 = arg0->f40;
    arg0->fE2 = arg0->f42;
    arg0->fE4 = arg0->f44;
    func_8004A9A4(arg0);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A9A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A9E4);

extern void *func_800474B4(int a, int b);

void func_8004AA70(unsigned char *obj, int a1, int a2)
{
    obj[0x11B] = 0;
    *(void **)(obj + 0x120) = func_800474B4(*(int *)(obj + 0x64), a1);
    *(void **)(obj + 0x124) = func_800474B4(*(int *)(obj + 0x64), a2);
}

typedef struct Part0x8004aabc {
    unsigned char pad0[0x40];
    short a[3];
    unsigned char pad46[0x6C - 0x46];
    short b[3];
} Part0x8004aabc;
typedef struct Obj0x8004aabc {
    unsigned char pad0[0x120];
    Part0x8004aabc *parts[2];
} Obj0x8004aabc;
extern void func_8004AC08(Obj0x8004aabc *obj);

void func_8004AABC(Obj0x8004aabc *obj)
{
    Part0x8004aabc **pp;
    Part0x8004aabc *p;
    unsigned int i;

    func_8004AC08(obj);
    i = 0;
    pp = obj->parts;
    for (; i < 2; i++) {
        p = *pp++;
        p->b[0] = p->a[0];
        p->b[1] = p->a[1];
        p->b[2] = p->a[2];
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004AB14);

extern int func_80035A88(int a, int b);

unsigned char func_8004ABB4(void)
{
    int i = func_80035A88(WORK->xC2D, WORK->xC2C);
    return ++WORK->xE6C[i];
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004AC08);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004ADD0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004AFC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004B298);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004B35C);

#define SHARED_F0 func_800487B8
void func_8004B428(void)
{
#include "../shared/func_8001EA54.inc.c"
}

#define SHARED_F0 func_800487B8
void func_8004B448(void)
{
#include "../shared/func_8001EA54.inc.c"
}

#define SHARED_F0 func_800487B8
void func_8004B468(void)
{
#include "../shared/func_8001EA54.inc.c"
}

#define SHARED_F0 func_800487B8
void func_8004B488(void)
{
#include "../shared/func_8001EA54.inc.c"
}

#define SHARED_F0 func_800487B8
void func_8004B4A8(void)
{
#include "../shared/func_8001EA54.inc.c"
}




extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


