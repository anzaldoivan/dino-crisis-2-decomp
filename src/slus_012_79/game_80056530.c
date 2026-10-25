#include "common.h"
#include "dc2.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;





extern void func_8004D51C();


extern void func_8004FF38(int x, short y, short w, short h, unsigned short idx);
void func_80050398(short x, short y, unsigned short w, unsigned short h, unsigned int color, unsigned short abe, unsigned short idx);
typedef void (*Line0x8004ce64)(int x0, int y0, int x1, int y1, int color, int abe, int idx);


void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);


extern void func_8004C960();
extern void func_8004D274();
extern void func_8004DFF4();
extern void func_8004F710(int a, int b, int c);
extern void func_8004F9A4(int a, int b);


void func_800501C4(void *p, int a, int b, int c, int d, int e, int f);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);


extern unsigned char D_800ABA96;
extern unsigned int D_800AC074[][0x400];
void func_80044834(void *, int);
void func_8004DB98(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
extern void func_80050030();
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_8007E194(u32 *p, int a, int b, int c, int d);


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
extern void func_8007E194();


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
extern void func_8007E194();


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern u8 D_800ABA96;
extern u32 D_800AC070[][0x400];


extern unsigned int D_800AC200[][0x400];
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
int func_80079AA4(SVECTOR *r, MATRIX *m);


extern void func_80050030();


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];


extern int func_80034CD8(int, int);


extern int func_80034CD8(int, int);


extern int func_80051C80(void *a, int b, int c);


extern int func_80051C80(void *a, int b, int c);




extern void func_80018020(void *, int);
extern void func_80034C48(int, int, int);


extern void func_8004D274(); /* was extern void func_8004D274(Obj_80052AB8 *); T8 w17 splice */


extern unsigned short D_800ABA90;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern int func_8001ED9C(int);
extern int func_80034CD8(int, int);
void func_80044834(void *, int);
extern void func_800544FC();
extern void func_80055518();


extern unsigned char D_800ABA96;
extern unsigned short D_800ABA90;
extern unsigned int D_800AC074[][0x400];
extern int func_8001D9D4(void);
void func_80044834(void *, int);
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
extern void func_8007E194();
extern void func_80053EAC();


extern unsigned char D_800ABA96;
extern unsigned short D_800ABA90;
extern unsigned int D_800AC074[][0x400];
extern int func_8001D9D4(void);
void func_80044834(void *, int);
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
extern void func_8007E194();
extern void func_80053EAC();


extern unsigned char D_800ABA96;
extern unsigned short D_800ABA68;
extern u32 D_800AC200[][0x400];
unsigned short func_8003DD34(int a, int b, int c);
void func_800501C4(void *p, int a, int b, int c, int d, int e, int f);
int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_80054A68(int x, int y);
void func_80054B60(int x, int y);
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_800798A4(MATRIX *m);
void func_800798D4(MATRIX *m);
unsigned int func_80079954(SVECTOR *v, int *xy, int *p, int *flag);
int func_80079AA4(SVECTOR *r, MATRIX *m);
void func_8007E194(u32 *p, int a, int b, int c, int d);


struct Box0x8002243c;
extern char D_8008A89C[];
int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_800514C4(struct Box0x8002243c *r, unsigned short idx, short n, unsigned short size, unsigned int col);


extern int func_80051CD4(void *a, int b);
extern int func_80051B3C(int a, int b);
extern void func_8004D274(); /* was extern void func_8004D274(Obj_80052AB8 *); T8 w17 splice */


extern unsigned short D_800ABA90;
extern char D_8008A82C[];
extern char D_8008A854[];
extern int func_80034CD8(int, int);
void func_80044834(void *, int);
extern void func_8004C960();
extern void func_8004DFF4();
extern void func_8004F450();
extern void func_8004F710(int a, int b, int c);
extern void func_8004F9A4(int a, int b);
int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
extern void func_80051BAC();


INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80056530", func_80056530);

typedef struct {
    union {
        int w;
        unsigned char b[4];
    } f0;
    char pad4[0x44 - 0x4];
    unsigned short f44;
    char pad46[0x4E - 0x46];
    signed char f4E;
    char pad4f[0x53 - 0x4F];
    signed char f53;
    char pad54[0x88 - 0x54];
    short f88;
} Obj56724;
extern unsigned short D_800ABA90;
extern char D_8008A89C[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern int func_8001ED9C(int);
extern int func_80034CD8(int, int);
void func_80044834(void *, int);
extern int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_800514C4(struct Box0x8002243c *r, unsigned short idx, short n, unsigned short size, unsigned int col);
extern void func_80057F48(Obj56724 *);
extern int func_800640B8(unsigned char);

void func_80056724(Obj56724 *p)
{
    int c1, c2;

    if (p->f44 & 0x5000) {
        func_80044834(0, 0);
        p->f4E ^= 1;
    } else if (D_800ABA90 & 0x40) {
        func_80044834(0, 1);
        p->f4E = 0;
        p->f0.b[2] = 6;
    } else if (D_800ABA90 & 0xA0) {
        if (p->f4E) {
            while (func_8001D9D4()) {
                func_8001CDE4(1);
            }
            func_8001ED9C((unsigned short)p->f53);
            p->f0.b[2] = 8;
            func_80044834(0, 2);
        } else {
            if (func_80034CD8(10, p->f53) && func_800640B8(p->f53)) {
                WORK->x6EA = p->f53;
                p->f0.w = 0x502;
            } else {
                p->f0.b[2] = 10;
            }
        }
        p->f0.b[3] = 0;
    }

    c2 = c1 = 0x303030;
    if (p->f4E) {
        c2 = 0x808080;
    } else {
        c1 = 0x808080;
    }
    func_800506BC(D_8008A89C + 24, 0xC9, 0x16, 100, c1, 2, 0);
    func_800506BC(D_8008A89C + 12, 0xC9, 0x26, 100, c2, 2, 0);

    if (++p->f88 >= 0x45) {
        p->f88 = 0;
    }

    {
        Box0x8002243c r;
        r = (Box0x8002243c){200, p->f4E * 16 + 0x16, 48, 16};
        func_800514C4(&r, p->f88, 0x22, 2, 0xFFFF);
    }
    {
        Box0x8002243c r;
        r = (Box0x8002243c){200, p->f4E * 16 + 0x16, 48, 16};
        func_800514C4(&r, p->f88 + 0x22, 0x22, 2, 0xFFFF);
    }
    func_80057F48(p);
}

extern Box0x8002243c D_80019F38;
extern unsigned char D_800ABA96;
extern unsigned short D_800ABA90;
extern unsigned int D_800AC074[][0x400];
extern int func_8001D9D4(void);
void func_80044834(void *, int);
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
extern void func_8007E194();
extern void func_80057A8C();

void func_80056A18(Obj0x80053648 *p)
{
    Box0x8002243c r;

    switch (p->x3) {
    case 0:
        p->x4E = 0;
        if (func_8001D9D4() == 0) {
            p->x3++;
        }
        break;
    case 1:
        r = D_80019F38;
        func_80050C90((unsigned short *)&r, 0, 0xC0, 0x7C40, 0x808080, 0, p->x3);
        func_8007E194(PKT, 1, 0, 0x8C, 0);
        ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr;
        ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr = (unsigned int)PKT;
        PKT += 3;
        if (D_800ABA90 & 0x40) {
            func_80044834(0, 1);
            p->x2 = 6;
            p->x3 = 0;
        }
        break;
    }
    func_80057A8C(p);
}

/* T8 w31: Blk8 typedef + D_80019F30 extern moved above func_80056B90 (was below it) so its draft splices with the same type */
extern Blk8x0x80053db0 D_80019F30;

typedef struct Obj0x80056b90 {
    char pad0[2];
    unsigned char x2;
} Obj0x80056b90;
extern unsigned short D_800ABA90;
extern Blk8x0x80053db0 D_80019F30;
extern char D_8008A97A[];
void func_80044834(void *, int);
extern void func_80050030();
extern int func_80050934();
int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
extern void func_8004D028();

void func_80056B90(Obj0x80056b90 *obj)
{
    unsigned short pad = D_800ABA90;
    Blk8x0x80053db0 tmp;

    if (pad & 0x40) {
        func_80044834(0, 1);
        obj->x2 = 0xB;
    } else if (pad & 0xA0) {
        func_80044834(0, 2);
        obj->x2 = 0xB;
    }
    tmp = D_80019F30;
    func_80050030(&tmp, 0x404040, 3, 2);
    func_800506BC(D_8008A97A, func_80050934(D_8008A97A), 0x6C, 0x64, 0x808080, 1, 0);
    func_8004D028(obj, 0xA0, 0x60);
}

typedef struct Obj0x80056c80 {
    char pad0[2];
    unsigned char x2;
    char pad3[0x50];
    unsigned char x53;
} Obj0x80056c80;
extern unsigned short D_800ABA90;
extern char D_8008A9B0[];
extern char D_8008A9E0[];
extern void func_80044834(void *, int);
extern void func_80050030();
extern int func_80050934();
int func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
extern void func_8004D028();

void func_80056C80(Obj0x80056c80 *obj)
{
    unsigned short pad = D_800ABA90;
    unsigned char k = obj->x53;
    Blk8x0x80053db0 tmp;

    if (pad & 0x40) {
        func_80044834(0, 1);
        obj->x2 = 6;
    } else if (pad & 0xA0) {
        func_80044834(0, 2);
        obj->x2 = 6;
    }
    tmp = D_80019F30;
    func_80050030(&tmp, 0x404040, 3, 2);
    if (k == 0x2D) {
        if (WORK->xC2D == 7) {
            func_800506BC(D_8008A9E0, func_80050934(D_8008A9E0), 0x6C, 0x64, 0x808080, 1, 0);
        } else {
            func_800506BC(D_8008A9B0, func_80050934(D_8008A9B0), 0x6C, 0x64, 0x808080, 1, 0);
        }
    } else if ((unsigned)(k - 0x37) < 2 || k == 0x33 || k == 0x34) {
        func_800506BC(D_8008A9E0, func_80050934(D_8008A9E0), 0x6C, 0x64, 0x808080, 1, 0);
    } else {
        func_800506BC(D_8008A9B0, func_80050934(D_8008A9B0), 0x6C, 0x64, 0x808080, 1, 0);
    }
    func_8004D028(obj, 0xA0, 0x60);
}

typedef struct Obj0x80056dfc {
    char pad0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
} Obj0x80056dfc;
extern char D_8008A7F8[];

void func_80056DFC(Obj0x80056dfc *p)
{
    p->x1 = 0;
    p->x2 = 0;
    p->x3 = 0;
    func_8004C960(p);
    func_8004DFF4(p, 0x80, 0x18, 2);
    func_8004F9A4(0x10, 0x58);
    func_8004F710(0x80, 0x58, 0x808080);
    func_8004F450(p);
    func_80050D80(D_80184A40, 0x60, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80184B60, 0xE8, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_800506BC(D_8008A7F8, 0x20, 0xC5, 0x64, 0x707070, 0x64, 0);
}

typedef struct Obj0x80056f00 {
    char pad0[0x70];
    unsigned short a;
    unsigned short b;
} Obj0x80056f00;
typedef struct { char s[43]; } Str43x0x80056f00;
typedef struct { char s[53]; } Str53x0x80056f00;
extern Str43x0x80056f00 D_80019F40;
extern Str53x0x80056f00 D_80019F6C;
extern void func_80051054();
extern int func_80050A90(char *s, int x, int y, int z, int a, int b);

void func_80056F00(Obj0x80056f00 *obj)
{
    Str43x0x80056f00 s1;
    Str53x0x80056f00 s2;

    obj->a = (obj->a + 0x50) & 0xFFF;
    func_80051054(obj->a, 0x44, 0xAC, 6, 8, 0x800000);
    obj->b = (obj->b + 0x14) & 0xFFF;
    func_80051054(obj->b, 0x44, 0xAC, 0xC, 0xE, 0x80);
    s1 = D_80019F40;
    func_80050A90(s1.s, 0x18, 0x80, 0xC8, 0x606060, 1);
    s2 = D_80019F6C;
    func_80050A90(s2.s, 0x32, 0xC8, 0xC8, 0x606060, 1);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80056530", func_80057178);



extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


