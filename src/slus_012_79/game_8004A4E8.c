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

typedef struct Obj0x8004a554 {
    char pad00[0x40];
    char sub40[0x10];
    unsigned int flags50;
    struct Obj0x8004a554 *next54;
    char pad58[0xC];
    struct Obj0x8004a554 *child64;
} Obj0x8004a554;
extern int func_8004A4FC();

void func_8004A554(Obj0x8004a554 *obj)
{
    Obj0x8004a554 *p;
    int on;
    unsigned int flags;

    flags = obj->flags50;
    if ((flags & 5) == 5) {
        on = func_8004A4FC(obj->sub40);
        if (on != 0) {
            obj->flags50 |= 2;
        } else {
            obj->flags50 &= ~2;
        }
    } else {
        on = flags >> 1;
        on &= 1;
    }
    for (p = obj->child64; p != 0; p = p->next54) {
        if (on != 0) {
            p->flags50 |= 2;
        } else {
            p->flags50 &= ~2;
        }
    }
}

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

typedef struct Shot0x8004a790 {
    char pad0[0x50];
    int flags50;
    char pad54[0x70 - 0x54];
} Shot0x8004a790;
extern int D_800AF150;
extern void func_8004A554();
extern void func_8004A610();

void func_8004A790(void)
{
    Unit *u;
    Shot0x8004a790 *s;
    int n;

    n = D_800AF11C;
    u = WORK->units;
    while (n--) {
        func_8004A554(u);
        u++;
    }
    n = D_800AF150;
    s = (Shot0x8004a790 *)WORK->x4DC;
    while (n--) {
        if ((s->flags50 & 5) == 5) {
            func_8004A610(s);
        }
        s++;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A4E8", func_8004A84C);

typedef struct Obj0x8004a8c0 {
    char pad0[0x50];
    int f50;
    char pad54[0x6C - 0x54];
    int f6C;
    int f70;
    int f74;
    char pad78[0xF8 - 0x78];
    unsigned char fF8;
    unsigned char fF9;
    unsigned char fFA;
} Obj0x8004a8c0;
extern int func_80034CD8(int, int);

void func_8004A8C0(Obj0x8004a8c0 *obj)
{
    if (WORK->x4C7 != 0) {
        if (obj->fF8 == 0) {
            goto set;
        }
        obj->fF8 -= 8;
        obj->fF9 -= 8;
        obj->fFA -= 8;
        obj->f6C -= 0x40;
        obj->f70 -= 0x100;
        obj->f74 -= 0x40;
    } else if (func_80034CD8(2, 0x18) != 0) {
set:
        obj->f50 |= 0x40000000;
    }
}

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

typedef struct Obj0x8004ab14 {
    char pad0[0x40];
    short x;                                      
    char pad42[2];
    short z;                                      
    char pad46[0xE];
    struct Obj0x8004ab14 *next;                   
    char pad58[0xC];
    struct Obj0x8004ab14 *child;                  
    char pad68[4];
    short ox;                                     
    char pad6e[2];
    short oz;                                     
    char pad72[0x76];
    short dx;                                     
    char padea[2];
    short dz;                                     
    char padee[0x2D];
    unsigned char flag;                            
    char pad11c[4];
    struct Obj0x8004ab14 *ref[4];                  
} Obj0x8004ab14;

int func_8004AB14(Obj0x8004ab14 *o)
{
    Obj0x8004ab14 *p;
    int dx;
    int dz;
    unsigned char f;
    int i;

    f = o->flag;
    i = f >> 4;
    if (f == 0) {
        return 0;
    }
    p = o->ref[i & 3];
    dx = p->x - p->ox;
    dz = p->z - p->oz;
    p = o->child;
    do {
        p->x -= o->dx + dx;
        p->z -= o->dz + dz;
        p = p->next;
    } while (p != 0);
    o->x -= dx;
    o->z -= dz;
    return 1;
}

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


