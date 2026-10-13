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

int func_8004A2C4(void *p, int arg)
{
    short *a = (short *)p;
    short *b = (short *)arg;
    return func_8007AC34(b[0] - a[0], b[2] - a[2]) & 0xFFF;
}

int func_8004A2F8(short *a)
{
    return func_8007AC34(a[2] - a[0], a[3] - a[1]) & 0xFFF;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A2C4", func_8004A32C);

int func_8004A378(short *p, short *q)
{
    int x0;
    int dz;
    int dx1;
    int px;
    int dz1;
    int dx3;
    int dz3;
    int z0;
    int dx2;
    int dz2;

    x0 = q[0];
    z0 = q[1];
    dz = p[2] - z0;
    dx1 = q[2] - x0;
    px = p[0] - x0;
    dz1 = q[3] - z0;
    dx3 = q[6] - x0;
    dz3 = q[7] - z0;
    if (dx1 * dz > dz1 * px || dx3 * dz < dz3 * px) {
        return 0;
    }
    dz2 = q[5] - z0;
    dx2 = q[4] - x0;
    dz -= dz2;
    dx1 -= dx2;
    px -= dx2;
    dz1 -= dz2;
    dx3 -= dx2;
    dz3 -= dz2;
    if (dx1 * dz < dz1 * px || dx3 * dz > dz3 * px) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004A2C4", func_8004A464);


extern int func_8004A4FC();


extern void func_8004A9A4();


extern void *func_800474B4(int a, int b);




extern int func_80035A88(int a, int b);


#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8




extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


