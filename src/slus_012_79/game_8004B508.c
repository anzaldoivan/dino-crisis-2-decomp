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




extern void *func_800474B4(int a, int b);




extern int func_80035A88(int a, int b);


#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

#define SHARED_F0 func_800487B8

typedef struct Wk0x8004b508 {
    char pad0[0x4E0];
    Unit0x80023d8c *x4E0;
    char pad4e4[0xC50 - 0x4E4];
    unsigned short xC50;
    char padc52[0xD90 - 0xC52];
    int xD90;
    char padd94[0xDBC - 0xD94];
    unsigned char xDBC;
    unsigned char xDBD;
    unsigned char xDBE;
    unsigned char xDBF;
} Wk0x8004b508;
typedef struct Obj0x8004b508 {
    unsigned char x0;
    char pad1[0x40 - 1];
    int x40;
    unsigned short x44;
    short x46;
    char pad48[2];
    short x4A;
    char pad4c[0x65 - 0x4C];
    signed char x65;
    char pad66[0x6C - 0x66];
    int x6C;
    char pad70[0xCE - 0x70];
    unsigned char xCE;
    unsigned char xCF;
} Obj0x8004b508;
extern void func_8001BE08();
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_80031084();
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80044834(void *, int);
extern void func_8004526C();
extern int func_8004FE9C();
extern void func_80079944();
extern void func_8007E0B4();
extern int D_800A79F0;
extern unsigned short D_800ABA90;
extern unsigned char D_800ABA96;
extern int D_800ABA98;
extern void (*D_8008A108[])(Obj0x8004b508 *);
extern char D_800AD06C[];
#define W0x8004b508 ((Wk0x8004b508 *)WORK)

void func_8004B508(void)
{
    Obj0x8004b508 *o;

    o = (Obj0x8004b508 *)((char *)WORK + 0x22C);
    func_80034C48(2, 0x10, 0);
    func_80034C48(3, 0, 0);
    *(int *)&o->x0 = 0;
    o->x6C = D_800A79F0;
    func_80034C48(2, 1, 0);
    func_80034C48(2, 0xD, 0);
    func_80034C48(2, 4, 1);
    func_8001BE08();
    o->x40 = W0x8004b508->xD90;
    o->xCE = 0;
    o->xCF = 0;
    o->x46 = W0x8004b508->xC50;
    o->x4A = W0x8004b508->x4E0->x108;
    func_80031084();
    func_80079944(0xC0);
    func_8004526C();
    func_80044834(0, 6);
    o->x65 = 0x1E;
    if (func_80034CD8(W0x8004b508->xDBE, W0x8004b508->xDBF)) {
        W0x8004b508->xDBE = 0;
        W0x8004b508->xDBF = 0;
        W0x8004b508->xDBC = 0;
        W0x8004b508->xDBD = 0;
    }
    func_8001CDE4(1);
    for (;;) {
        D_800A79F0 = 0;
        if (o->x65 >= 0) {
            if (o->x65 == 0) {
                if ((D_800ABA90 & 0x100) && func_8001D9D4() == 0) {
                    if ((unsigned char)W0x8004b508->xC50 != 0) {
                        *(int *)&o->x0 = 0x502;
                    } else {
                        *(int *)&o->x0 = 0x90102;
                    }
                }
            } else {
                o->x65--;
            }
        }
        o->x44 = func_8004FE9C(o);
        o->x44 = (o->x44 & 0xF000) | (D_800ABA90 & 0xFFF);
        D_8008A108[(signed char)o->x0](o);
        func_8007E0B4(D_800ABA98 + 0x78, D_800AD06C + (D_800ABA96 << 12), D_800AD06C - 0xFFC + (D_800ABA96 << 12));
        func_8001CDE4(1);
    }
}

unsigned char func_8004B778(unsigned char a, unsigned char b)
{
    if (a >= 4) {
        if (a >= 5) {
            a++;
        } else {
            a = 4;
            if ((unsigned)(b - 7) < 4) {
                a = 5;
            }
        }
    }
    return a;
}




extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


