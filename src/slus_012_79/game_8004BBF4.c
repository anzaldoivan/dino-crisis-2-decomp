#include "common.h"
#include "dc2.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;




typedef struct Obj0x8004bbf4 {
    char pad0;
    signed char state;
} Obj0x8004bbf4;
extern void func_8004D51C();
extern void func_8004BA5C(Obj0x8004bbf4 *obj);
extern void (*D_8008A118[])(Obj0x8004bbf4 *obj);

void func_8004BBF4(Obj0x8004bbf4 *obj)
{
    if (obj->state != 5) {
        func_8004D51C();
    }
    func_8004BA5C(obj);
    D_8008A118[obj->state](obj);
}

typedef struct Obj0x8004bc58 {
    char pad0;
    unsigned char x1;
    unsigned char x2;
    char pad3[0x44 - 0x3];
    unsigned short x44;
    char pad46[0x4D - 0x46];
    signed char x4D;
    char pad4e;
    unsigned char x4F;
    char pad50[0x60 - 0x50];
    signed char x60;
    char pad61;
    unsigned char x62;
    unsigned char x63;
    char pad64[0x84 - 0x64];
    short x84;
    short x86;
    short x88;
} Obj0x8004bc58;
extern short D_800ABA90;
extern void func_8004262C(int);
extern void func_80042758(int);
extern void func_80044834(void *, int);
extern int func_8004B778();
extern void func_8004BFC8(unsigned short, unsigned short, unsigned short, unsigned short);
extern void func_8004EEE4(Obj0x8004bc58 *);
extern void func_8004F450();
extern void func_800514C4(Box0x8002243c *, unsigned short, int, int, int);

void func_8004BC58(Obj0x8004bc58 *obj)
{
    Box0x8002243c a, b, c;
    int i;
    int v;

    if (obj->x44 & 0x1000) {
        func_80044834(0, 0);
        obj->x84 = 0;
        obj->x86 = 0;
        if (--obj->x4D < 0) {
            obj->x4D = 3;
        }
    } else if (obj->x44 & 0x4000) {
        func_80044834(0, 0);
        obj->x84 = 0;
        obj->x86 = 0;
        if (++obj->x4D >= 4) {
            obj->x4D = 0;
        }
    } else if (D_800ABA90 & 0x40) {
        obj->x84 = 0;
        obj->x86 = 0;
        obj->x1 = 5;
        obj->x2 = 0;
    } else if (D_800ABA90 & 0xA0) {
        obj->x84 = 250;
        obj->x86 = 250;
        obj->x2 = 0;
        obj->x4F = 0;
        obj->x1 = obj->x4D + 1;
        func_80044834(0, 2);
        i = obj->x4D + 1;
        if (i == 3) {
            if (WORK->x6E8 != i) {
                func_8004262C(3);
                WORK->x6E8 = i;
            }
        }
        if (obj->x4D + 1 == 4) {
            v = func_8004B778(WORK->xC2D, WORK->xC2C);
            obj->x62 = v;
            obj->x60 = v + 5;
            obj->x63 = WORK->xC2C;
            if (WORK->x6E8 != obj->x60) {
                func_80042758(0);
                func_8004262C(obj->x60);
                WORK->x6E8 = obj->x60;
            }
        }
    }
    func_8004EEE4(obj);
    func_8004F450(obj);
    obj->x84 += obj->x86;
    if (obj->x84 > 250) {
        obj->x84 = 250;
    } else {
        obj->x86 += 3;
    }
    func_8004BFC8(obj->x4D, (obj->x4D * 8 + 0x68) & ~7, (obj->x4D * 16 + 0x78) & ~7, obj->x84 - 50);
    if (++obj->x88 > 100) {
        obj->x88 = 0;
    }
    b.w = 80;
    b.h = 16;
    b.x = obj->x4D * 8 + 20;
    b.y = obj->x4D * 16 + 120;
    a = b;
    func_800514C4(&a, obj->x88, 50, 2, 0xFFFF);
    c.w = 80;
    c.h = 16;
    c.x = obj->x4D * 8 + 20;
    c.y = obj->x4D * 16 + 120;
    b = c;
    func_800514C4(&b, obj->x88 + 50, 50, 2, 0xFFFF);
}

extern const Lbl0x8004f450 D_80019B50;

void func_8004BFC8(unsigned short arg0, unsigned short arg1, unsigned short arg2, unsigned short arg3)
{
    Lbl0x8004f450 strs;

    strs = D_80019B50;
    func_80050D80(D_80184A40, 0x60, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80184B60, 0xE8, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_800506BC(strs.s[arg0], 0x20, 0xC5, 0x64, 0x707070, 0x64, 0);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004BBF4", func_8004C0B4);

typedef struct Obj0x8004c6dc {
    char pad0;
    signed char x1;
    char pad2[0x38 - 0x2];
    short x38;
    short x3A;
    short x3C;
    short x3E;
    char pad40[0x46 - 0x40];
    unsigned short x46;
    char pad48[0x53 - 0x48];
    signed char x53;
    char pad54[0x64 - 0x54];
    unsigned char x64;
    char pad65[0x6C - 0x65];
    int x6C;
} Obj0x8004c6dc;
typedef struct Rec0x8004c6dc {
    char pad0[6];
    unsigned short x6;
    char pad8[8];
} Rec0x8004c6dc;
typedef struct Sub0x8004c6dc {
    char pad0[0x40];
    Rec0x8004c6dc *x40;
    char pad44[0x4C - 0x44];
    unsigned char x4C;
    char pad4d;
    unsigned char x4E;
} Sub0x8004c6dc;
extern int D_800A79F0;
extern unsigned short D_800ABA68;
extern short D_800ABA90;
extern void func_8002F398();
extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern int func_8001D9D4(void);
extern void func_8001D9E4(void);
extern void func_8001DB64();
extern void func_8001DBFC(void);
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern void func_80042108();
extern void func_8005AD28();
extern void func_80079944();

void func_8004C6DC(Obj0x8004c6dc *obj)
{
    int state;
    Sub0x8004c6dc *p;
    Sub0x8004c6dc *q;
    int old;
    unsigned short c;

    state = obj->x1;
    switch (state) {
    case 0:
        func_8001D9E4();
        while (func_8001D9D4() != 0) {
            func_8001CDE4(1);
        }
        obj->x1++;
        break;
    case 1:
        c = WORK->xC50;
        if (obj->x46 != c) {
            func_80042108(c & 0xFF, c >> 8);
        }
        if (WORK->x6E8 != 0 && *(unsigned short *)&WORK->xC2C != 0x905) {
            p = (Sub0x8004c6dc *)WORK->x318;
            WORK->x6E9 = state;
            old = p->x4C;
            p->x4C = p->x4E;
            WORK->x3E7 = 0xFF;
            func_8001DB64(old);
            func_8001DBFC();
        }
        func_8002EB14(1, 1, 1);
        func_8001CDE4(1);
        WORK->x6E8 = 0;
        D_800A79F0 = obj->x6C;
        if (WORK->xC2D == 7) {
            WORK->x6F8 = 0x70;
            WORK->x6F9 = 0x70;
            WORK->x6FA = 0x70;
            WORK->x6FB = 0;
            WORK->x6FC = 0;
        }
        func_80034C48(3, 0, 1);
        func_80034C48(2, 8, 0);
        if (WORK->x6EA != 0xFF) {
            func_80034C48(9, obj->x53, 0);
        } else if (obj->x64 != 4) {
            func_8002EB14(0, 0xF, 1);
        }
        obj->x64 = 0;
        obj->x38 = 0;
        obj->x3A = 0;
        obj->x3C = 0;
        obj->x3E = 0;
        q = (Sub0x8004c6dc *)WORK->x318;
        func_80079944(((Rec0x8004c6dc *)((char *)q->x40 + (q->x4C << 4)))->x6);
        func_80034C48(2, 4, ((signed char *)WORK)[0x108C]);
        func_8005AD28();
        ((short *)&WORK)[4] = 0;
        ((short *)&WORK)[5] = 0;
        D_800ABA68 = 0;
        D_800ABA90 = 0;
        func_8001CED4((int)func_8002F398);
        break;
    }
}

extern void func_8004C960();
extern void func_8004D274();
extern void func_8004DFF4();
extern void func_8004F710(int a, int b, int c);
extern void func_8004F9A4(int a, int b);


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern u8 D_800ABA96;
extern u32 D_800AC070[][0x400];


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern int func_80051C80(void *a, int b, int c);


extern int func_80051C80(void *a, int b, int c);






extern unsigned char D_800ABA96;
extern unsigned short D_800ABA68;
extern u32 D_800AC200[][0x400];
unsigned short func_8003DD34(int a, int b, int c);
void func_800501C4(void *p, int a, int b, int c, int d, int e, int f);
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_80054A68(int x, int y);
void func_80054B60(int x, int y);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_800798A4(MATRIX *m);
void func_800798D4(MATRIX *m);
unsigned int func_80079954(SVECTOR *v, int *xy, int *p, int *flag);
void func_80079AA4(SVECTOR *r, MATRIX *m);
void func_8007E194(u32 *p, int a, int b, int c, int d);


extern int func_80051CD4(void *a, int b);
extern int func_80051B3C(int a, int b);




extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


