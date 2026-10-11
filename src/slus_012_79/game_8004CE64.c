#include "common.h"
#include "dc2.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;




typedef struct { char *s; char pad4[6]; unsigned short xA; } Str12; /* was { char *s; char pad4[8]; } at func_80053300 site; xA: func_80053300 (T8 w23) */
extern Str12 D_80019770[]; /* moved up from the func_80053300 site: func_80053300 reads it */

extern void func_8004D51C();


extern void func_8004FF38();
void func_80050398(short x, short y, unsigned short w, unsigned short h, unsigned int color, unsigned short abe, unsigned short idx);
typedef void (*Line0x8004ce64)(int x0, int y0, int x1, int y1, int color, int abe, int idx);

void func_8004CE64(unsigned short x, unsigned short y, unsigned short h, signed char dir)
{
    unsigned char i;
    short yy;

    func_8004FF38(0, 0, 0x140, 0xF0, 100);
    ((Line0x8004ce64)func_80050398)((short)x, (short)y, (short)x, (short)(y + 0x40), 0x808080, 0, 100);
    for (i = 0; i < 20; i++) {
        yy = (unsigned short)(h - 8) + y + i * 4;
        if (yy > y + 0x40) {
            break;
        }
        if (i & 1) {
            ((Line0x8004ce64)func_80050398)((short)x, yy, (short)(x + dir * 4), yy, 0x808080, 0, 100);
        } else {
            ((Line0x8004ce64)func_80050398)((short)x, yy, (short)(x + dir * 8), yy, 0x606060, 0, 100);
        }
    }
    if (dir == -1) {
        x -= 8;
    }
    func_8004FF38((short)x, (short)y, 9, 0x38, 100);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004D028);

typedef struct Obj0x8004d274 {
    signed char b0;
    signed char b1;
    char pad2[0x64 - 2];
    unsigned char x64;
} Obj0x8004d274;
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);

void func_8004D274(Obj0x8004d274 *p)
{
    if (WORK->xC2B == 1) {
        func_80050D80(D_80185008, 0x88, 0xE, 0xC, 0x7C8F, 0x8068C040, 2);
    } else {
        func_80050D80(D_80185000, 0x88, 0xE, 0xC, 0x7C8F, 0x8068C040, 2);
    }
    func_80050D80(D_80183798, 8, 0x78, 0xD, 0x7C8E, 0x80808080, 500);
    func_80050D80(D_80183CD8, 0x78, 0x40, 0xD, 0x7C8E, 0x181818, 1000);
    func_80050D80(D_80183928, 0x138, 0x78, 0xD, 0x7C8E, 0x80808080, 500);
    func_80050D80(D_801854D0, 0x84, 0xD8, 0xC, 0x7C8F, 0x808080, 0x14);
    if (((signed char *)WORK)[0x108B] == 1) {
        func_80050D80(D_801854D8, 0x84, 0xD8, 0xC, 0x7C8E, 0xC04040, 2);
    } else {
        func_80050D80(D_801854E0, 0x84, 0xD8, 0xC, 0x7C8E, 0x40C040, 2);
    }
    if (p->b0 == 2 && p->b1 != 0 && p->b1 != 5) {
        if (p->x64) {
            func_80050D80(D_801850B8, 0x9A, 0xDB, 0xC, 0x7C8F, 0x8068C040, p->b0);
        } else {
            func_80050D80(D_80185040, 0x98, 0xDB, 0xC, 0x7C8F, 0x8068C040, p->b0);
        }
    } else {
        func_80050D80(D_801850B8, 0x9A, 0xDB, 0xC, 0x7C8F, 0x8068C040, 2);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004D51C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004D964);

extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
void func_8007E194(u32 *p, int a, int b, int c, int d);

int func_8004DB98(int x, int y, unsigned short n, unsigned short lead, unsigned int color, unsigned int val, unsigned short idx)
{
    short rect[4];
    unsigned int div;
    unsigned short i;
    unsigned short d;
    int cnt;
    unsigned int col;
    unsigned int abe;

    div = 1;
    col = color & 0xFFFFFF;
    abe = color >> 31;
    cnt = 0;
    for (i = 1; i < n; i++) {
        div *= 10;
    }
    for (i = 0; i < n; i++) {
        d = val / div;
        if (d) {
            lead = 1;
        }
        val -= d * div;
        div /= 10;
        if (div == 0) {
            lead = 1;
        }
        if (lead || d) {
            rect[0] = x + i * 8;
            rect[1] = y;
            rect[2] = 8;
            rect[3] = 10;
            ((void (*)())func_80050C90)(rect, (short)(d << 3), 0, 0x7C8F, col, abe, idx);
            cnt++;
        }
    }
    func_8007E194(PKT, 1, 0, 12, 0);
    ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)&D_800AC070[D_800ABA96][idx])->addr;
    ((Tag0x8004c960 *)&D_800AC070[D_800ABA96][idx])->addr = (unsigned int)PKT;
    PKT += 3;
    return cnt;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004DDC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004DFF4);

typedef struct Obj0x8004eee4 {
    unsigned char pad0[0x64];
    unsigned char x64;
} Obj0x8004eee4;
extern void func_8004C960();
extern void func_8004D274();
extern void func_8004DFF4();
extern void func_8004F710(int a, int b, int c);
extern void func_8004F9A4(int a, int b);

void func_8004EEE4(Obj0x8004eee4 *obj)
{
    func_8004C960(obj);
    func_8004F9A4(0x10, 0x58);
    func_8004F710(0x80, 0x58, 0x808080);
    if (obj->x64 == 0) {
        func_8004DFF4(obj, 0x80, 0x18, 2);
    }
    func_8004D274(obj);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004EF50);

typedef struct Quad0x8004f450 {
    short x0, y0, x1, y1, x2, y2, x3, y3;
} Quad0x8004f450;
/* Lbl0x8004f450: dc2.h (T8 w35) */
typedef struct Obj0x8004f450 {
    char pad0[0x4D];
    signed char sel;
    char pad4E[0x84 - 0x4E];
    unsigned short t;
} Obj0x8004f450;
extern Quad0x8004f450 D_80019D20;
extern Quad0x8004f450 D_80019D30;
extern Lbl0x8004f450 D_80019D40;
void func_800501C4(void *p, int a, int b, int c, int d, int e, int f);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);

void func_8004F450(Obj0x8004f450 *obj)
{
    Quad0x8004f450 q1;
    Quad0x8004f450 q2;
    Lbl0x8004f450 lbl;
    unsigned char i;
    int c;
    int d;

    q1 = D_80019D20;
    q2 = D_80019D30;
    lbl = D_80019D40;

    for (i = 0; i < 4; i++) {
        c = 0x2000;
        if (obj->sel == i) {
            c = (((short)obj->t / 2) << 16) | (((short)obj->t / 2 + 0x20) << 8) | ((short)obj->t / 2);
        }
        func_80050D80(lbl.s[i], i * 8 + 0x14, i * 16 + 0x78, 0xC, 0x7C8F, c, 2);
        if (obj->sel == i) {
            c = (((short)obj->t / 4 + 0x56) << 16) | (((short)obj->t / 2 + 0x6D) << 8) | ((short)obj->t / 4 + 0x14);
            d = 0x2000;
        } else {
            c = 0x2B5F2F;
            d = 0x2000;
        }
        q1.x0 = q1.x2 = i * 8 + 0x10;
        q2.x0 = q2.x2 = q1.x1 = q1.x3 = i * 8 + 0x3C;
        q2.x1 = q2.x3 = i * 8 + 0x68;
        q1.y0 = q1.y1 = q2.y0 = q2.y1 = i * 16 + 0x78;
        q1.y2 = q1.y3 = q2.y2 = q2.y3 = i * 16 + 0x88;
        func_800501C4(&q1, d, c, d, c, 1, 100);
        func_800501C4(&q2, c, d, c, d, 1, 100);
    }
}

typedef struct Sub0x8004f710 {
    char pad0[0x40];
    int x40;
} Sub0x8004f710;
typedef struct Work0x8004f710 {
    char pad0[4];
    int x4;
    char pad8[0x22C - 0x8];
    Sub0x8004f710 x22C;
    char pad270[0xD90 - 0x270];
    int xD90;
} Work0x8004f710;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC074[][0x400];
void func_80044834(void *, int);
int func_8004DB98(int x, int y, unsigned short n, unsigned short lead, unsigned int color, unsigned int val, unsigned short idx); /* was void func_8004DB98(int a0..a6): w30 c449 draft returns a count */
extern void func_80050030();
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_8007E194(u32 *p, int a, int b, int c, int d);

void func_8004F710(int a, int b, int c)
{
    Work0x8004f710 *w;
    Sub0x8004f710 *s;
    int d;
    Rect0x8006c2d8 r0;
    Rect0x8006c2d8 r1;

    w = (Work0x8004f710 *)WORK;
    s = &w->x22C;
    d = s->x40 - w->xD90;
    if (d != 0) {
        if ((w->x4 & 3) == 0) {
            func_80044834(0, 0);
        }
        if (d > 3 || d < -3) {
            d = d / 3;
        } else if (d > 0) {
            d = 1;
        } else {
            d = -1;
        }
        s->x40 -= d;
    }
    r0.x = a + 0x18;
    r0.y = b;
    r0.w = 0x80;
    r0.h = 0x10;
    func_80050030(&r0, 0x274C11, 1, 3);
    func_8004DB98(a + 0x48, b + 4, 7, 1, c, s->x40, 1);
    r1.x = a + 0x84;
    r1.y = b + 4;
    r1.w = 0x10;
    r1.h = 0xA;
    func_80050C90((unsigned short *)&r1, 0x50, 0, 0x7C8F, c, 0, 1);
    ((void (*)())func_8007E194)(PKT, 1, 0, 0xC, 0);
    ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr;
    ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr = (unsigned int)PKT;
    PKT += 3;
    func_80050D80(D_801854C0, a + 8, b, 0xC, 0x7C8E, 0x808080, 2);
    func_80050D80(D_801854C8, a + 0x98, b, 0xC, 0x7C8E, 0x808080, 2);
    func_80050D80(D_80185010, a + 0x18, b + 1, 0xC, 0x7C8F, 0x808080, 2);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004F9A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004FE9C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8004FF38);

typedef struct Tile0x80050030 {
    unsigned int tag;
    unsigned int code;
    unsigned short x, y;
    unsigned short w, h;
} Tile0x80050030;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
extern void func_8007E194();

void func_80050030(void *pv, int a, int b, int c)
{
    unsigned short *p = pv;
    unsigned short abe = b;
    unsigned short idx = c;
    unsigned int *q;
    unsigned int *ot;

    ((Tile0x80050030 *)PKT)->x = p[0];
    ((Tile0x80050030 *)PKT)->y = p[1];
    ((Tile0x80050030 *)PKT)->w = p[2];
    ((Tile0x80050030 *)PKT)->h = p[3];
    if (abe) {
        ((Tile0x80050030 *)PKT)->code = a | 0x62000000;
    } else {
        ((Tile0x80050030 *)PKT)->code = a | 0x60000000;
    }
    ot = &D_800AC070[D_800ABA96][idx];
    q = PKT;
    *q = *ot | 0x03000000;
    *ot = (unsigned int)q & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x10);
    if (abe) {
        func_8007E194(PKT, 1, 0, ((abe - 1) << 5) | 0xC, 0);
        *PKT = (*PKT & 0xFF000000) | (*(D_800AC070[D_800ABA96] + idx) & 0xFFFFFF);
        *(D_800AC070[D_800ABA96] + idx) = (*(D_800AC070[D_800ABA96] + idx) & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
        PKT = (unsigned int *)((unsigned char *)PKT + 0xC);
    }
}

typedef struct G4x0x800501c4 {
    unsigned int tag;
    unsigned int code;
    unsigned short x0, y0;
    unsigned int c1;
    unsigned short x1, y1;
    unsigned int c2;
    unsigned short x2, y2;
    unsigned int c3;
    unsigned short x3, y3;
} G4x0x800501c4;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];
extern void func_8007E194();

void func_800501C4(void *pv, int a, int b, int c, int d, int e, int f)
{
    unsigned short *p = pv;
    unsigned short abe = e;
    unsigned short idx = f;
    unsigned int *q;
    unsigned int *ot;

    ((G4x0x800501c4 *)PKT)->x0 = p[0];
    ((G4x0x800501c4 *)PKT)->y0 = p[1];
    ((G4x0x800501c4 *)PKT)->x1 = p[2];
    ((G4x0x800501c4 *)PKT)->y1 = p[3];
    ((G4x0x800501c4 *)PKT)->x2 = p[4];
    ((G4x0x800501c4 *)PKT)->y2 = p[5];
    ((G4x0x800501c4 *)PKT)->x3 = p[6];
    ((G4x0x800501c4 *)PKT)->y3 = p[7];
    if (abe) {
        ((G4x0x800501c4 *)PKT)->code = a | 0x3A000000;
    } else {
        ((G4x0x800501c4 *)PKT)->code = a | 0x38000000;
    }
    ((G4x0x800501c4 *)PKT)->c1 = b;
    ((G4x0x800501c4 *)PKT)->c2 = c;
    ((G4x0x800501c4 *)PKT)->c3 = d;
    ot = &D_800AC070[D_800ABA96][idx];
    q = PKT;
    *q = *ot | 0x08000000;
    *ot = (unsigned int)q & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x24);
    func_8007E194(PKT, 1, 0, 0xD, 0);
    *PKT = (*PKT & 0xFF000000) | (*(D_800AC070[D_800ABA96] + idx) & 0xFFFFFF);
    *(D_800AC070[D_800ABA96] + idx) = (*(D_800AC070[D_800ABA96] + idx) & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT = (unsigned int *)((unsigned char *)PKT + 0xC);
}

extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];

void func_80050398(short x, short y, unsigned short w, unsigned short h, unsigned int color, unsigned short abe, unsigned short idx)
{
    unsigned int *p;
    unsigned int *ot;

    ((Tile0x80023cec *)PKT)->x = x;
    ((Tile0x80023cec *)PKT)->y = y;
    ((Tile0x80023cec *)PKT)->w = w;
    ((Tile0x80023cec *)PKT)->h = h;
    if (abe) {
        ((Tile0x80023cec *)PKT)->code = color | 0x42000000;
    } else {
        ((Tile0x80023cec *)PKT)->code = color | 0x40000000;
    }
    ot = &D_800AC070[D_800ABA96][idx];
    p = PKT;
    *p = *ot | 0x03000000;
    *ot = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x10);
}

extern char D_8008A5C4[];
extern void func_80050030();
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);

void func_80050454(unsigned short x, unsigned short y, unsigned short sel, unsigned short idx) {
    unsigned int c0, c1;

    if (sel == 0) {
        c0 = 0x808080;
        c1 = 0x404040;
    } else {
        c0 = 0x404040;
        c1 = 0x808080;
    }
    func_800506BC(D_8008A5C4, (unsigned short)(x + 9), y, 100, c0, idx, 0);
    func_800506BC(D_8008A5C4 + 8, (unsigned short)(x + 0x52), y, 100, c1, idx, 0);
    if (sel == 0) {
        Box0x8002243c r;
        r.x = x;
        r.y = y;
        r.w = 0x30;
        r.h = 0x10;
        func_80050030(&r, 0x8000, 1, idx);
    } else {
        Box0x8002243c r;
        r.x = x + 0x44;
        r.y = y;
        r.w = 0x30;
        r.h = 0x10;
        func_80050030(&r, 0x8000, 1, idx);
    }
}

typedef struct Ft40x80050588 {
    unsigned int tag;
    unsigned int code;
    unsigned short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    unsigned short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    unsigned short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} Ft40x80050588;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];

void func_80050588(unsigned short x, unsigned short y, unsigned int color, unsigned short i)
{
    unsigned short idx = i;
    unsigned int *q;
    unsigned int *ot;

    ((Ft40x80050588 *)PKT)->code = (color & 0xFFFFFF) | 0x2C000000;
    ((Ft40x80050588 *)PKT)->x0 = ((Ft40x80050588 *)PKT)->x2 = x;
    ((Ft40x80050588 *)PKT)->y0 = ((Ft40x80050588 *)PKT)->y1 = y;
    ((Ft40x80050588 *)PKT)->x1 = ((Ft40x80050588 *)PKT)->x3 = ((Ft40x80050588 *)PKT)->x0 + 15;
    ((Ft40x80050588 *)PKT)->y2 = ((Ft40x80050588 *)PKT)->y3 = ((Ft40x80050588 *)PKT)->y0 + 15;
    ((Ft40x80050588 *)PKT)->u0 = ((Ft40x80050588 *)PKT)->u2 = 0x70;
    ((Ft40x80050588 *)PKT)->v0 = ((Ft40x80050588 *)PKT)->v1 = 0x70;
    ((Ft40x80050588 *)PKT)->u1 = ((Ft40x80050588 *)PKT)->u3 = ((Ft40x80050588 *)PKT)->u0 + 15;
    ((Ft40x80050588 *)PKT)->v2 = ((Ft40x80050588 *)PKT)->v3 = ((Ft40x80050588 *)PKT)->v0 + 15;
    ((Ft40x80050588 *)PKT)->tpage = 0x28;
    ((Ft40x80050588 *)PKT)->clut = 0x7DCD;
    ot = &D_800AC070[D_800ABA96][idx];
    q = PKT;
    *q = *ot | 0x09000000;
    *ot = (unsigned int)q & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x28);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_800506BC);

extern unsigned char D_8008A488[];

int func_80050934(unsigned short *s)
{
    unsigned short cur = 0;
    unsigned short max = 0;
    unsigned short c;
    unsigned short hi;

    for (;;) {
        c = *s;
        hi = c >> 12;
        if (hi == 4) {
            if (max < cur) {
                max = cur;
            }
            s++;
            cur = 0;
            continue;
        }
        if (hi == 15) {
            break;
        }
        cur = cur + D_8008A488[(c & 0xFFF) >> 1];
        s++;
    }
    if (max < cur) {
        max = cur;
    }
    max = max >> 1;
    return (unsigned short)(160 - max);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_800509C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80050A40);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80050A90);

extern u8 D_800ABA96;
extern u32 D_800AC070[][0x400];

void func_80050C90(u16 *rect, u8 u, u8 v, u16 clut, u32 color, u16 abe, u16 idx)
{
    u32 *p;
    u32 *ot;

    ((Sprt *)PKT)->x = rect[0];
    ((Sprt *)PKT)->y = rect[1];
    ((Sprt *)PKT)->w = rect[2];
    ((Sprt *)PKT)->h = rect[3];
    ((Sprt *)PKT)->u = u;
    ((Sprt *)PKT)->v = v;
    ((Sprt *)PKT)->clut = clut;
    if (abe) {
        ((Sprt *)PKT)->code = color | 0x66000000;
    } else {
        ((Sprt *)PKT)->code = color | 0x64000000;
    }
    ot = &D_800AC070[D_800ABA96][idx];
    p = PKT;
    *p = *ot | 0x04000000;
    *ot = (u32)p & 0xFFFFFF;
    PKT = (u32 *)((u8 *)PKT + 0x14);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80050D80);

void func_80050F50(unsigned short x, unsigned short y)
{
    int c;
    int m;

    m = 0x80000000;
    c = WORK->x4;
    c &= 0x1E;
    c *= 8;
    func_80050D80(D_80185028, x, y, 0xC, 0x7C8F, ((c + 0xA0) << 8) | ((c + 0xA0) | m), 2);
    func_80050D80(D_80185028, x + 6, y, 0xC, 0x7C8F, ((c + 0x50) << 8) | ((c + 0x50) | m), 2);
    func_80050D80(D_80185028, x + 12, y, 0xC, 0x7C8F, (c << 8) | (c | m), 2);
}

typedef struct {
    short vx, vy;
} Pt0x80051054;
typedef struct {
    unsigned int tag;
    unsigned int c0;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} PolyF40x80051054;
extern unsigned int D_800AC200[][0x400];
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_80079AA4(SVECTOR *r, MATRIX *m);

void func_80051054(short ang, unsigned short x, unsigned short y, int r0, int r1, int col)
{
    SVECTOR v;
    SVECTOR rot;
    MATRIX m;
    Pt0x80051054 a[65];
    Pt0x80051054 b[65];
    unsigned int i;
    unsigned int *ot;
    unsigned int *p;

    rot.vx = 0;
    rot.vy = ang;
    rot.vz = 0;
    for (i = 0; i < 64; i++) {
        v.vy = 0;
        v.vx = 0;
        v.vz = r0 * 400;
        func_80079AA4(&rot, &m);
        func_80079704(&m, &v, &v);
        a[i].vx = x + v.vx / 400;
        a[i].vy = y + v.vz / 400;
        v.vy = 0;
        v.vx = 0;
        v.vz = (r1 + 2) * 400;
        func_80079AA4(&rot, &m);
        func_80079704(&m, &v, &v);
        b[i].vx = x + v.vx / 400;
        b[i].vy = y + v.vz / 400;
        rot.vy += 0x40;
    }
    a[64] = a[0];
    for (i = 0; i < 64; i++) {
        if (!(i & 0x10)) {
            ((PolyF40x80051054 *)PKT)->x0 = a[i].vx;
            ((PolyF40x80051054 *)PKT)->x1 = a[i + 1].vx;
            ((PolyF40x80051054 *)PKT)->x2 = b[i].vx;
            ((PolyF40x80051054 *)PKT)->x3 = b[i + 1].vx;
            ((PolyF40x80051054 *)PKT)->y0 = a[i].vy;
            ((PolyF40x80051054 *)PKT)->y1 = a[i + 1].vy;
            ((PolyF40x80051054 *)PKT)->y2 = b[i].vy;
            ((PolyF40x80051054 *)PKT)->y3 = b[i + 1].vy;
            ((PolyF40x80051054 *)PKT)->c0 = (col + (i << 11)) | 0x28000000;
            *PKT = 0x05000000;
            ot = D_800AC200[D_800ABA96];
            p = PKT;
            *p = *ot | 0x05000000;
            *ot = (unsigned int)p & 0xFFFFFF;
            PKT = (unsigned int *)((unsigned char *)PKT + 0x18);
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_800513C0);

extern void func_80050030();

void func_800514C4(Box0x8002243c *r, unsigned short idx, short n, unsigned short size, unsigned int col)
{
    Box0x8002243c rc;
    Box0x8002243c tmp;
    unsigned int qw, qh, i;
    unsigned int c0, c1, c2;
    unsigned short x0, y0;
    unsigned int color;
    unsigned int cnt;

    cnt = n;
    qw = r->w / size;
    qh = r->h / size;
    r->x -= size;
    r->y -= size;
    c0 = col & 0xFF;
    c1 = (col >> 8) & 0xFF;
    c2 = (col >> 16) & 0xFF;
    for (i = 0; i < n; i++) {
        x0 = r->x;
        y0 = r->y;
        tmp.x = x0 + size * idx;
        tmp.y = y0;
        tmp.w = size;
        tmp.h = size;
        rc = tmp;
        idx = idx % ((qw + qh) * 2 + 4);
        color = ((c2 * i / cnt) << 16) | ((c1 * i / cnt) << 8) | (c0 * i / cnt);
        if (idx < qw + 2) {
            rc.x = x0 + size * idx;
            rc.y = y0;
        } else if (idx < qw + qh + 3) {
            rc.x = x0 + r->w + size;
            rc.y = y0 + (idx - qw - 1) * size;
        } else if (idx < qw * 2 + qh + 4) {
            rc.y = y0 + r->h + size;
            rc.x = x0 + r->w - (idx - qw - qh - 3) * size;
        } else {
            rc.x = x0;
            rc.y = y0 + r->h - (idx - qw - qh - qw - 4) * size;
        }
        func_80050030(&rc, color, 1, 1);
        idx++;
    }
}

typedef struct {
    unsigned int tag;
    unsigned int c0;
    unsigned short x0, y0;
    unsigned short x1, y1;
    unsigned short x2, y2;
} PolyF30x800517b4;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];

void func_800517B4(unsigned short *xy, int c0, unsigned short idx)
{
    unsigned int *p;
    unsigned int *ot;
    unsigned int code;

    ((PolyF30x800517b4 *)PKT)->x0 = xy[0];
    ((PolyF30x800517b4 *)PKT)->y0 = xy[1];
    ((PolyF30x800517b4 *)PKT)->x1 = xy[2];
    ((PolyF30x800517b4 *)PKT)->y1 = xy[3];
    ((PolyF30x800517b4 *)PKT)->x2 = xy[4];
    ((PolyF30x800517b4 *)PKT)->y2 = xy[5];
    code = c0 & 0xFFFFFF;
    if (c0 < 0) {
        ((PolyF30x800517b4 *)PKT)->c0 = code | 0x22000000;
    } else {
        ((PolyF30x800517b4 *)PKT)->c0 = code | 0x20000000;
    }
    ot = &D_800AC070[D_800ABA96][idx];
    p = PKT;
    *p = *ot | 0x04000000;
    *ot = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x14);
}

typedef struct {
    unsigned int tag;
    unsigned int c0;
    unsigned short x0, y0;
    unsigned int c1;
    unsigned short x1, y1;
    unsigned int c2;
    unsigned short x2, y2;
} PolyG30x8005189c;
extern unsigned char D_800ABA96;
extern unsigned int D_800AC070[][0x400];

void func_8005189C(unsigned short *xy, int c0, int c1, int c2, unsigned short idx)
{
    unsigned int *p;
    unsigned int *ot;
    unsigned int code;

    ((PolyG30x8005189c *)PKT)->x0 = xy[0];
    ((PolyG30x8005189c *)PKT)->y0 = xy[1];
    ((PolyG30x8005189c *)PKT)->x1 = xy[2];
    ((PolyG30x8005189c *)PKT)->y1 = xy[3];
    ((PolyG30x8005189c *)PKT)->x2 = xy[4];
    ((PolyG30x8005189c *)PKT)->y2 = xy[5];
    code = c0 & 0xFFFFFF;
    if (c0 < 0) {
        ((PolyG30x8005189c *)PKT)->c0 = code | 0x32000000;
    } else {
        ((PolyG30x8005189c *)PKT)->c0 = code | 0x30000000;
    }
    ((PolyG30x8005189c *)PKT)->c1 = c1;
    ((PolyG30x8005189c *)PKT)->c2 = c2;
    ot = &D_800AC070[D_800ABA96][idx];
    p = PKT;
    *p = *ot | 0x06000000;
    *ot = (unsigned int)p & 0xFFFFFF;
    PKT = (unsigned int *)((unsigned char *)PKT + 0x1C);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051998);

extern int func_80034CD8(int, int);

int func_80051B3C(int start, int end)
{
    unsigned char n = 0;
    unsigned char i = start;
    unsigned char e = end;

    for (; i < e; i++) {
        if (func_80034CD8(9, i)) {
            n++;
        }
    }
    return n;
}

extern int func_80034CD8(int, int);

void func_80051BAC(int start, int end, int max, unsigned char *p)
{
    unsigned char n = 0;
    unsigned char i = start;
    unsigned char e = end;
    unsigned char m = max;

    for (; i < e; i++) {
        if (func_80034CD8(9, i)) {
            *p++ = i;
            if (++n >= m) {
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051C38);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051C80);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051CD4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051D18);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051D58);

typedef struct Rec0x80051dc4 {
    unsigned char b0;
    char pad1[3];
    int x4;
    int x8;
} Rec0x80051dc4;
extern void func_80018020(void *, int);

void func_80051DC4(Rec0x80051dc4 *p, unsigned char n)
{
    unsigned char i;
    unsigned char j;
    Rec0x80051dc4 *q;

    for (i = 0; i < n - 1; i++, p++) {
        q = p + 1;
        if (p->b0 == 0) {
            for (j = i + 1; j < n; j++, q++) {
                if (q->b0 != 0) {
                    *p = *q;
                    func_80018020(q, 3);
                    break;
                }
            }
        }
    }
}

typedef struct Rec0x80051e88 {
    unsigned char b0;
    unsigned char b1;
    char pad2[6];
    short x8;
    short xA;
} Rec0x80051e88;
typedef struct Work0x80051e88 {
    char pad0[0xD48];
    Rec0x80051e88 xD48[6];
} Work0x80051e88;
#define WORK0x80051e88 (*SCRATCH(Work0x80051e88 *, 0))
extern void func_80018020(void *, int);
extern void func_80051DC4();

void func_80051E88(void)
{
    unsigned short i;
    unsigned short j;
    Rec0x80051e88 *e;
    Rec0x80051e88 *p;

    for (i = 0; i < 5; i++) {
        e = &WORK0x80051e88->xD48[i];
        if (e->b0 == 0) {
            continue;
        }
        p = &WORK0x80051e88->xD48[5];
        for (j = 5; i < j; j--, p--) {
            if (p->b0 != 0 && p->b1 == e->b1) {
                if (e->x8 < e->xA) {
                    e->x8++;
                    if (--p->x8 <= 0) {
                        func_80018020(p, 3);
                        func_80051DC4(WORK0x80051e88->xD48, 6);
                    }
                    break;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80051F8C);

typedef struct Rec0x80052364 {
    unsigned char b0;
    unsigned char b1;
    char pad2[6];
    short x8;
    char padA[2];
} Rec0x80052364;
typedef struct Work0x80052364 {
    char pad0[0xC30];
    unsigned char xC30;
    char padc31[0xC50 - 0xC31];
    unsigned char xC50;
    char padc51[0xC58 - 0xC51];
    Rec0x80052364 xC58[1];
} Work0x80052364;
#define WORK0x80052364 (*SCRATCH(Work0x80052364 *, 0))
extern int func_80051C80(void *a, int b, int c);

int func_80052364(unsigned short a0)
{
    unsigned char i;
    int r;

    if (WORK0x80052364->xC30 & 1) {
        r = a0;
    } else {
        i = func_80051C80(WORK0x80052364->xC58, 0x14, WORK0x80052364->xC50);
        if (WORK0x80052364->xC58[i].x8 < a0) {
            r = 0;
        } else {
            r = a0;
        }
    }
    return r;
}

typedef struct Rec0x800523ec {
    unsigned char b0;
    unsigned char b1;
    char pad2[6];
    short x8;
    char padA[2];
} Rec0x800523ec;
typedef struct Work0x800523ec {
    char pad0[0xC30];
    unsigned char xC30;
    char padc31[0xC50 - 0xC31];
    unsigned short xC50;
    char padc52[0xC58 - 0xC52];
    Rec0x800523ec xC58[1];
} Work0x800523ec;
#define WORK0x800523ec (*SCRATCH(Work0x800523ec *, 0))
extern int func_80051C80(void *a, int b, int c);

int func_800523EC(unsigned short a0)
{
    unsigned char i;
    int r;

    if (WORK0x800523ec->xC30 & 1) {
        r = a0;
    } else {
        i = func_80051C80(WORK0x800523ec->xC58, 0x14, (WORK0x800523ec->xC50 >> 8) | 0x10);
        if (WORK0x800523ec->xC58[i].x8 < a0) {
            r = 0;
        } else {
            r = a0;
        }
    }
    return r;
}

typedef struct Rec0x8005247c {
    char pad0[4];
    int x4;
    unsigned short x8;
    char padA[2];
} Rec0x8005247c;
typedef struct Work0x8005247c {
    char pad0[0x224];
    int x224;
    char pad228[0x22C - 0x228];
    unsigned char x22C[0x30C - 0x22C];
    char pad30c[0xC30 - 0x30C];
    unsigned char xC30;
    char padc31[0xC50 - 0xC31];
    unsigned char xC50;
    char padc51[0xC58 - 0xC51];
    Rec0x8005247c xC58[1];
} Work0x8005247c;
#define WORK0x8005247c (*SCRATCH(Work0x8005247c *, 0))
extern int func_80051C80(void *a, int b, int c);

int func_8005247C(unsigned short a0)
{
    unsigned char *p;
    unsigned char i;

    p = WORK0x8005247c->x22C;
    p[0xCE] = 100;
    p[0xCF] = 0;
    i = func_80051C80(WORK0x8005247c->xC58, 0x14, WORK0x8005247c->xC50);
    if (!(WORK0x8005247c->xC30 & 1)) {
        WORK0x8005247c->xC58[i].x8 -= a0;
    }
    WORK0x8005247c->xC58[i].x4 += a0;
    WORK0x8005247c->x224++;
    return a0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8005254C);

void func_80052634(void)
{
    WORK->x228++;
}

void func_80052654(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8005265C);

typedef struct { int w[21]; } Rec7x0x800526b4;
typedef struct { int w[18]; } Rec6x0x800526b4;
typedef struct { int w[9]; } Rec3x0x800526b4;
typedef struct { int w[12]; } Rec4x0x800526b4;
extern Rec7x0x800526b4 D_80019D68;
extern Rec6x0x800526b4 D_80019DBC;
extern Rec6x0x800526b4 D_80019E04;
extern Rec3x0x800526b4 D_80019E4C;
extern Rec4x0x800526b4 D_80019E70;
extern Rec4x0x800526b4 D_80019EA0;
extern void func_80018020(void *, int);
extern void func_80034C48(int, int, int);

void func_800526B4(void)
{
    Rec7x0x800526b4 a;
    Rec6x0x800526b4 b;
    Rec6x0x800526b4 c;
    Rec3x0x800526b4 d;
    Rec4x0x800526b4 e;
    Rec4x0x800526b4 f;

    a = D_80019D68;
    b = D_80019DBC;
    c = D_80019E04;
    func_80018020(WORK->xC58, 0x3C);
    if (*(signed char *)WORK->padc2a == 0) {
        *(Rec7x0x800526b4 *)WORK->xC58 = a;
    } else if (*(signed char *)WORK->padc2a == 1) {
        *(Rec6x0x800526b4 *)WORK->xC58 = b;
    } else {
        *(Rec6x0x800526b4 *)WORK->xC58 = c;
    }
    d = D_80019E4C;
    e = D_80019E70;
    f = D_80019EA0;
    func_80018020(&WORK->xC58[20], 0x12);
    if (*(signed char *)WORK->padc2a == 0) {
        *(Rec3x0x800526b4 *)&WORK->xC58[20] = d;
    } else if (*(signed char *)WORK->padc2a == 1) {
        *(Rec4x0x800526b4 *)&WORK->xC58[20] = e;
    } else {
        *(Rec4x0x800526b4 *)&WORK->xC58[20] = f;
    }
    func_80034C48(9, 0x74, 1);
}

typedef struct {
    signed char b0;
    signed char b1;
    signed char b2;
} Obj_80052AB8;
extern void (*D_8008A138[])(Obj_80052AB8 *);
extern void func_8004C960(Obj_80052AB8 *);
extern void func_8004D274(); /* was extern void func_8004D274(Obj_80052AB8 *); T8 w17 splice */
extern void func_8004DFF4(Obj_80052AB8 *, int, int, int);

void func_80052AB8(Obj_80052AB8 *p)
{
    D_8008A138[p->b2](p);
    func_8004C960(p);
    func_8004D274(p);
    if (p->b2 != 7 && p->b2 != 4) {
        func_8004DFF4(p, 0x80, 0x18, 2);
    }
}

typedef struct Obj0x80052b34 {
    char pad0[2];
    unsigned char x2;
    unsigned char x3;
    int x4;
    char pad8[0x10];
    char x18[8];
    char x20[8];
    char pad28[0x20];
    short x48;
    char pad4A[2];
    unsigned char x4C;
    char pad4D;
    unsigned char x4E;
    char pad4F[0xC];
    unsigned char x5B;
    unsigned char x5C;
} Obj0x80052b34;
extern char D_8008A772[];
extern void func_8004F450();
extern void func_8004F710(int a, int b, int c);
extern void func_8004F9A4(int a, int b);
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
extern void *func_80081CA4();

void func_80052B34(Obj0x80052b34 *p)
{
    p->x2 = 1;
    p->x3 = 0;
    p->x4 = 0;
    p->x4E = 0;
    p->x48 = 0;
    p->x4C = 0;
    p->x5B = 0;
    p->x5C = 0;
    func_80081CA4(p->x18, 0, 8);
    func_80081CA4(p->x20, 0, 8);
    func_8004F710(0x80, 0x58, 0x808080);
    func_8004F450(p);
    func_8004F9A4(0x10, 0x58);
    func_800506BC(D_8008A772, 0x20, 0xC5, 0x64, 0x808080, 1, 0);
    func_80050D80(D_80184A40, 0x60, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80184B60, 0xE8, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80052C58);

typedef struct Obj0x8005319c {
    char pad0[2];
    unsigned char x2;
    signed char x3;
    char pad4[0x44 - 0x4];
    unsigned short x44;
    char pad46[0x4C - 0x46];
    signed char x4C;
    char pad4d;
    unsigned char x4E;
    char pad4f[0x5B - 0x4F];
    signed char x5B;
} Obj0x8005319c;
extern unsigned short D_800ABA90;
void func_80044834(void *, int);
extern void func_800544FC();

void func_8005319C(Obj0x8005319c *p)
{
    unsigned short f;
    unsigned short k;

    if (p->x3 == 0) {
        f = p->x44;
        if (f & 0x8000) {
            if (p->x5B >= 2) {
                func_80044834(0, 8);
            }
            p->x3 = 0x10;
            if (--p->x4C < 0) {
                p->x4C = p->x5B - 1;
            }
        } else if (f & 0x2000) {
            if (p->x5B >= 2) {
                func_80044834(0, 8);
            }
            p->x3 = 0x10;
            if (++p->x4C > p->x5B - 1) {
                p->x4C = 0;
            }
        } else {
            k = D_800ABA90;
            if (k & 0x40) {
                func_80044834(0, 1);
                p->x2 = *(unsigned char *)&WORK->xC50 ? 1 : 9;
                p->x4E = 0;
            } else if (k & 0xA0) {
                func_80044834(0, 2);
                p->x2 = 3;
                p->x4E = 0;
            }
        }
    } else {
        p->x3--;
    }
    func_800544FC(p, 1);
}

typedef struct Obj0x80053300 {
    char pad0[2];
    unsigned char x2;
    unsigned char x3;
    int x4;
    char pad8[0x18 - 0x8];
    unsigned char x18[0x28];
    char pad40[0x44 - 0x40];
    unsigned short x44;
    char pad46[0x4C - 0x46];
    signed char x4C;
    char pad4d;
    signed char x4E;
    char pad4f[0x92 - 0x4F];
    short x92;
    short x94;
} Obj0x80053300;
extern unsigned short D_800ABA90;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ED9C(int);
extern int func_80034CD8(int, int);
void func_80044834(void *, int);
extern void func_800544FC();
extern void func_80055518();

void func_80053300(Obj0x80053300 *p)
{
    int v;
    unsigned char c;

    if (p->x44 & 0x5000) {
        func_80044834(0, 0);
        p->x4E ^= 1;
    } else if (D_800ABA90 & 0x40) {
        func_80044834(0, 1);
        p->x2 = 2;
        p->x3 = 0;
        p->x4 = 0;
    } else if (D_800ABA90 & 0xA0) {
        func_80044834(0, 2);
        if (p->x4E != 0) {
            while (func_8001D9D4()) {
                func_8001CDE4(1);
            }
            func_8001ED9C(WORK->xC58[p->x18[p->x4C]].b1);
            c = 4;
            p->x2 = c;
            p->x94 = 0;
        } else {
            v = WORK->xC58[p->x18[p->x4C]].b1;
            if (D_80019770[v].xA & 0x20) {
                WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1;
            } else {
                v = WORK->xC50;
                if (D_80019770[v & 0xFF].xA & 0x20) {
                    c = WORK->xC2B;
                    if (c == 0) {
                        if (func_80034CD8(9, 0x38)) {
                            WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x800;
                        } else {
                            WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x200;
                        }
                    } else if (c == 1) {
                        if (func_80034CD8(9, 0x37)) {
                            WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x700;
                        } else {
                            WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x100;
                        }
                    } else if (c == 6) {
                        WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x600;
                    } else if (c == 5) {
                        WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x600;
                    } else if (c == 2) {
                        WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | 0x500;
                    }
                } else {
                    WORK->xC50 = WORK->xC58[p->x18[p->x4C]].b1 | (v & 0xFF00);
                }
            }
            p->x2 = 2;
        }
        p->x92 = 0;
        p->x4 = 0;
    }
    func_800544FC(p, 1);
    func_80055518(p);
}

extern Box0x8002243c D_80019ED0;
extern unsigned char D_800ABA96;
extern unsigned short D_800ABA90;
extern unsigned int D_800AC074[][0x400];
extern int func_8001D9D4(void);
void func_80044834(void *, int);
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
extern void func_8007E194();
extern void func_80053EAC();

void func_80053648(Obj0x80053648 *p)
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
        r = D_80019ED0;
        func_80050C90((unsigned short *)&r, 0, 0xC0, 0x7C40, 0x808080, 0, p->x3);
        func_8007E194(PKT, 1, 0, 0x8C, 0);
        ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr;
        ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr = (unsigned int)PKT;
        PKT += 3;
        if (D_800ABA90 & 0x40) {
            func_80044834(0, 1);
            p->x2 = 2;
            p->x3 = 0;
        }
        break;
    }
    func_80053EAC(p, 1);
}

typedef struct Obj0x800537c8 {
    char pad0[2];
    unsigned char x2;
    signed char x3;
    char pad4[0x44 - 0x4];
    unsigned short x44;
    char pad46[0x48 - 0x46];
    short x48;
    char pad4a[0x4C - 0x4A];
    signed char x4C;
    char pad4d;
    unsigned char x4E;
    char pad4f[0x5C - 0x4F];
    signed char x5C;
} Obj0x800537c8;
extern unsigned short D_800ABA90;
void func_80044834(void *, int);
extern void func_800544FC();

void func_800537C8(Obj0x800537c8 *p)
{
    unsigned short f;
    unsigned short k;

    if (p->x3 == 0) {
        f = p->x44;
        if (f & 0x8000) {
            if (p->x5C >= 2) {
                func_80044834(0, 8);
            }
            p->x3 = 0x10;
            if (--p->x4C < 0) {
                p->x4C = p->x5C - 1;
            }
        } else if (f & 0x2000) {
            if (p->x5C >= 2) {
                func_80044834(0, 8);
            }
            p->x3 = 0x10;
            if (++p->x4C > p->x5C - 1) {
                p->x4C = 0;
            }
        } else {
            k = D_800ABA90;
            if (k & 0x40) {
                p->x48 = 0;
                p->x4C = 0;
                func_80044834(0, 1);
                p->x2 = 1;
                p->x4E = 1;
            } else if (k & 0xA0) {
                func_80044834(0, 2);
                p->x2 = 6;
                p->x4E = 0;
            }
        }
    } else {
        p->x3--;
    }
    func_800544FC(p, 0);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80053920);

extern unsigned char D_800ABA96;
extern unsigned short D_800ABA90;
extern unsigned int D_800AC074[][0x400];
extern int func_8001D9D4(void);
void func_80044834(void *, int);
void func_80050C90(unsigned short *rect, unsigned char u, unsigned char v, unsigned short clut, unsigned int color, unsigned short abe, unsigned short idx);
extern void func_8007E194();
extern void func_80053EAC();

void func_80053B1C(Obj0x80053648 *p)
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
        r = D_80019ED0;
        func_80050C90((unsigned short *)&r, 0, 0xC0, 0x7C40, 0x808080, 0, p->x3);
        func_8007E194(PKT, 1, 0, 0x8C, 0);
        ((Tag0x8004c960 *)PKT)->addr = ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr;
        ((Tag0x8004c960 *)D_800AC074[D_800ABA96])->addr = (unsigned int)PKT;
        PKT += 3;
        if (D_800ABA90 & 0x40) {
            func_80044834(0, 1);
            p->x2 = 5;
            p->x3 = 0;
        }
        break;
    }
    func_80053EAC(p, 0);
}

typedef struct Obj0x80053c9c {
    char pad0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    int x4;
} Obj0x80053c9c;
extern char D_8008A736[];

void func_80053C9C(Obj0x80053c9c *p)
{
    if ((unsigned char)WORK->xC50) {
        p->x2 = 0;
        p->x1 = 0;
    } else {
        p->x2 = 9;
    }
    p->x3 = 0;
    p->x4 = 0;
    func_8004F710(0x80, 0x58, 0x808080);
    func_8004F450(p);
    func_8004F9A4(0x10, 0x58);
    func_80050D80(D_80184A40, 0x60, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80184B60, 0xE8, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_800506BC(D_8008A736, 0x20, 0xC5, 0x64, 0x707070, 0x64, 0);
}

typedef struct Obj0x80053db0 {
    char pad0[2];
    unsigned char x2;
    char pad3[0x45];
    short x48;
    char pad4A[2];
    unsigned char x4C;
} Obj0x80053db0;
extern unsigned short D_800ABA90;
extern Blk8x0x80053db0 D_80019ED8;
extern char D_8008A904[];
void func_80044834(void *, int);
extern void func_80050030();
extern int func_80050934();
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
extern void func_8004D028();

void func_80053DB0(Obj0x80053db0 *obj)
{
    unsigned short pad;
    Blk8x0x80053db0 tmp;

    obj->x4C = 0;
    pad = D_800ABA90;
    obj->x48 = 0;
    if (pad & 0x40) {
        func_80044834(0, 1);
        obj->x2 = 2;
    } else if (pad & 0xA0) {
        func_80044834(0, 2);
        obj->x2 = 2;
    }
    tmp = D_80019ED8;
    func_80050030(&tmp, 0x404040, 3, 2);
    func_800506BC(D_8008A904, func_80050934(D_8008A904), 0x6C, 0x64, 0x808080, 1, 0);
    func_8004D028(obj, 0xA0, 0x60);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80053EAC);

typedef struct {
    char pad0[0x18];
    unsigned char x18[8];
    unsigned char x20[0x28];
    short x48;
    char pad4a[2];
    signed char x4C;
    char pad4d[0x5B - 0x4D];
    unsigned char x5B;
    unsigned char x5C;
} Sel;
typedef struct { unsigned char b[16]; } Blk16;
extern unsigned char D_800ABA96;
extern unsigned short D_800ABA68;
extern Blk16 D_80019EF0;
extern u32 D_800AC200[][0x400];
unsigned short func_8003DD34(int a, int b, int c);
void func_800501C4(void *p, int a, int b, int c, int d, int e, int f);
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_80054A68(int x, int y);
void func_80054B60(int x, int y);
void func_80054C58(Sel *p, int x, int y, int sel, int c, int col, int z);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_800798A4(MATRIX *m);
void func_800798D4(MATRIX *m);
unsigned int func_80079954(SVECTOR *v, int *xy, int *p, int *flag);
void func_80079AA4(SVECTOR *r, MATRIX *m);
void func_8007E194(u32 *p, int a, int b, int c, int d);

void func_800544FC(Sel *p, int mode)
{
    MATRIX m0;
    MATRIX m1;
    SVECTOR rot;
    SVECTOR v;
    Blk16 blk;
    int xy;
    int z;
    int flag;
    unsigned char n;
    unsigned char *cp;
    unsigned short d;
    unsigned short a;
    unsigned char i;
    unsigned int r;
    int b;
    int c;
    Rec12 *rec;
    u32 *pk;
    u32 *ot;

    if (mode == 1) {
        func_80050D80(D_801856A0, 0x18, 0x5D, 0xC, 0x7C8F, 0x808080, 2);
        n = p->x5B;
        cp = p->x18;
    } else {
        func_80050D80(D_801856B0, 0x18, 0x5D, 0xC, 0x7C8F, 0x808080, 2);
        cp = p->x20;
        n = p->x5C;
    }
    b = ((0x1000 / n) & 0xFFF) / 16;
    a = ((0x1000 / n) & 0xFFF) * -p->x4C;
    d = func_8003DD34(p->x48, b, a);
    if (((a - (u16)p->x48) & 0xFFF) == 0x800 && (D_800ABA68 & 0x8000)) {
        d = -d;
    }
    rot.vx = 0x8D0;
    rot.vz = 0;
    rot.vy = p->x48 += d;
    func_80079AA4(&rot, &m0);
    m0.t[0] = 0;
    m0.t[1] = -0x28;
    m0.t[2] = 0x401;
    rot.vz = 0;
    rot.vy = 0;
    rot.vx = 0;
    if (d == 0 && n >= 2) {
        func_80054A68(0xC8, 0xA8);
        func_80054B60(0x78, 0xA8);
    }
    for (i = 0; i < n; i++, cp++) {
        func_80079AA4(&rot, &m1);
        v.vx = 0;
        v.vy = 0;
        v.vz = 0x1B8;
        func_80079704(&m1, &v, &v);
        func_800798A4(&m0);
        func_800798D4(&m0);
        r = func_80079954(&v, &xy, &z, &flag);
        rot.vy = (rot.vy + 0x1000 / n) & 0xFFF;
        c = 0xA0 - r / 3;
        func_80054C58(p, (u16)xy, (u16)(xy >> 16), i == p->x4C, *cp, (c << 16) | (c << 8) | c, (u16)r);
        if (i == p->x4C) {
            rec = &WORK->xC58[*cp];
            blk = D_80019EF0;
            func_800501C4(&blk, 0x803000, 0x201000, 0x803000, 0x201000, 1, 100);
            func_8007E194(PKT, 1, 0, 12, 0);
            pk = PKT;
            *pk = (*pk & 0xFF000000) | (*D_800AC200[D_800ABA96] & 0xFFFFFF);
            ot = D_800AC200[D_800ABA96];
            *ot = (*ot & 0xFF000000) | ((u32)PKT & 0xFFFFFF);
            PKT += 3;
            func_800506BC(D_80019770[rec->b1].s, 0x20, 0xC9, 0x64, 0x808080, 1, 0);
        }
    }
    func_80050D80(D_80184A40, 0x60, 0xC8, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80184B60, 0xE8, 0xC8, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80185278, 0x88, 0x5D, 0xC, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80185328, 0x100, 0x65, 0xC, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_801853C8, 0x46, 0xBC, 0xC, 0x7C8F, 0x1080F0, 5);
    func_80050D80(D_801853D0, 0xD0, 0xBC, 0xC, 0x7C8F, 0x1080F0, 5);
}

#define M2C_UNK s32
s32 func_8004A84C(M2C_UNK, M2C_UNK, M2C_UNK);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);

void func_80054A68(s32 arg0, s32 arg1) {
    s32 temp_s2;
    s32 temp_s3;

    temp_s3 = arg0 & 0xFFFF;
    temp_s2 = arg1 & 0xFFFF;
    ((M2C_UNK (*)(M2C_UNK,s32,s32,M2C_UNK,s32,s32,s32))func_80050D80)(0x801859F0, temp_s3, temp_s2, 0x2C, 0x7C8F, func_8004A84C(0x800060A0, 0x80000840, 4), 3);
    ((M2C_UNK (*)(M2C_UNK,s32,s32,M2C_UNK,s32,s32,s32))func_80050D80)(0x801859F0, temp_s3 + 4, temp_s2, 0x2C, 0x7C8F, func_8004A84C(0x80004080, 0x80000840, 4), 3);
    ((M2C_UNK (*)(M2C_UNK,s32,s32,M2C_UNK,s32,s32,s32))func_80050D80)(0x801859F0, temp_s3 + 8, temp_s2, 0x2C, 0x7C8F, func_8004A84C(0x80002060, 0x80000840, 4), 3);
}

s32 func_8004A84C(M2C_UNK, M2C_UNK, M2C_UNK);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);

void func_80054B60(s32 arg0, s32 arg1) {
    s32 temp_s2;
    s32 temp_s3;

    temp_s3 = arg0 & 0xFFFF;
    temp_s2 = arg1 & 0xFFFF;
    ((M2C_UNK (*)(M2C_UNK,s32,s32,M2C_UNK,s32,s32,s32))func_80050D80)(0x801859E8, temp_s3 - 8, temp_s2, 0x2C, 0x7C8F, func_8004A84C(0x800060A0, 0x80000840, 4), 3);
    ((M2C_UNK (*)(M2C_UNK,s32,s32,M2C_UNK,s32,s32,s32))func_80050D80)(0x801859E8, temp_s3 - 0xC, temp_s2, 0x2C, 0x7C8F, func_8004A84C(0x80004080, 0x80000840, 4), 3);
    ((M2C_UNK (*)(M2C_UNK,s32,s32,M2C_UNK,s32,s32,s32))func_80050D80)(0x801859E8, temp_s3 - 0x10, temp_s2, 0x2C, 0x7C8F, func_8004A84C(0x80002060, 0x80000840, 4), 3);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80054C58);

typedef struct Obj0x80055518 {
    char pad0[0x4E];
    signed char x4E;
    char pad4F[0x88 - 0x4F];
    short x88;
} Obj0x80055518;
struct Box0x8002243c;
extern char D_8008A89C[];
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_800514C4(struct Box0x8002243c *r, unsigned short idx, short n, unsigned short size, unsigned int col);

void func_80055518(Obj0x80055518 *p)
{
    Box0x8002243c a;
    Box0x8002243c b;
    Box0x8002243c c;
    int c2;
    int c3;

    if (p->x4E != 0) {
        c2 = 0x303030;
        c3 = 0x808080;
    } else {
        c2 = 0x808080;
        c3 = 0x303030;
    }
    func_800506BC(D_8008A89C, 0x2E, 0xA0, 0x64, c2, 3, 0);
    func_800506BC(D_8008A89C + 0xC, 0x2E, 0xB2, 0x64, c3, 3, 0);
    func_80050D80(D_801850C8, 0x46, 0xA0, 0xC, 0x7C8E, c2, 4);
    func_80050D80(D_801850C8, 0x46, 0xB2, 0xC, 0x7C8E, c3, 4);
    if (++p->x88 >= 0x45) {
        p->x88 = 0;
    }
    b.x = 0x2E;
    b.y = p->x4E * 18 + 0xA0;
    b.w = 0x30;
    b.h = 0x10;
    a = b;
    func_800514C4((struct Box0x8002243c *)&a, (unsigned short)p->x88, 0x22, 2, 0xFFFF);
    c.x = 0x2E;
    c.y = p->x4E * 18 + 0xA0;
    c.w = 0x30;
    c.h = 0x10;
    b = c;
    func_800514C4((struct Box0x8002243c *)&b, (unsigned short)(p->x88 + 0x22), 0x22, 2, 0xFFFF);
}

typedef struct {
    char pad0[2];
    signed char x2;
    char pad3[0x50 - 3];
    unsigned char x50;
    char pad51[0x5D - 0x51];
    unsigned char x5D;
} Obj_80055724;
extern int func_80051CD4(void *a, int b);
extern int func_80051B3C(int a, int b);
extern void func_8004D274(); /* was extern void func_8004D274(Obj_80052AB8 *); T8 w17 splice */
extern void (*D_8008A160[])(Obj_80055724 *p);

void func_80055724(Obj_80055724 *p)
{
    p->x5D = func_80051CD4((char *)WORK + 0xD48, 6);
    p->x50 = func_80051B3C(0x20, 0x50);
    D_8008A160[p->x2](p);
    func_8004D274(p);
}

typedef struct Obj0x80055798 {
    char pad0[2];
    unsigned char x2;
    unsigned char x3;
    int x4;
    char pad8[0x20];
    char x28[8];
    char pad30[0x1E];
    unsigned char x4E;
    unsigned char x4F;
    char pad50;
    unsigned char x51;
    unsigned char x52;
    unsigned char x53;
    char pad54[6];
    unsigned char x5A;
    char pad5B[9];
    unsigned char x64;
    char pad65[0x2D];
    short x92;
} Obj0x80055798;
extern char D_8008A82C[];
extern void func_8004C960();
extern void func_8004DFF4();
extern void func_8004F450();
extern void func_8004F710(int a, int b, int c);
extern void func_8004F9A4(int a, int b);
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
extern void *func_80081CA4();

void func_80055798(Obj0x80055798 *p)
{
    if (p->x64 == 5) {
        p->x2 = 2;
    } else {
        p->x2 = 1;
    }
    p->x3 = 0;
    p->x4 = 0;
    p->x4E = 0;
    p->x5A = 0;
    p->x4F = 0;
    p->x51 = 0;
    p->x52 = 0;
    p->x53 = 0;
    p->x92 = 0x1E;
    func_80081CA4(p->x28, 0, 8);
    if (p->x64 != 5) {
        func_8004C960(p);
        func_8004DFF4(p, 0x80, 0x18, 2);
        func_8004F9A4(0x10, 0x58);
        func_8004F710(0x80, 0x58, 0x808080);
        func_8004F450(p);
        func_80050D80(D_80184A40, 0x60, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
        func_80050D80(D_80184B60, 0xE8, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
        func_800506BC(D_8008A82C, 0x20, 0xC5, 0x64, 0x808080, 0x64, 0);
    }
}

typedef struct Obj0x800558f4 {
    char pad0[2];
    signed char x2;
    signed char x3;
    int x4;
    char pad8[0x28 - 8];
    char x28[0x44 - 0x28];
    unsigned short x44;
    char pad46[0x4E - 0x46];
    signed char x4E;
    signed char x4F;
    signed char x50;
    char pad51;
    signed char x52;
    char pad53[0x5A - 0x53];
    signed char x5A;
    char pad5B[2];
    signed char x5D;
    char pad5E[0x88 - 0x5E];
    short x88;
    char pad8A[0x92 - 0x8A];
    short x92;
    short x94;
} Obj0x800558f4;
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
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
extern void func_80051BAC();

void func_800558F4(Obj0x800558f4 *p)
{
    Box0x8002243c a;
    Box0x8002243c b;
    Box0x8002243c c;
    unsigned char i;
    signed char n;
    signed char v;
    int c3;
    int c2;

    if (p->x44 & 0xF000) {
        func_80044834(0, 0);
        p->x5A ^= 1;
    } else if (D_800ABA90 & 0x40) {
        func_80044834(0, 1);
        p->x2 = 0xB;
        p->x3 = 0;
        p->x4 = 0;
    } else if (D_800ABA90 & 0xA0) {
        func_80044834(0, 2);
        if (p->x5A == 0) {
            if (p->x5D != 0) {
                p->x2 = 2;
                p->x92 = 0x1E;
                goto tail;
            }
            v = 5;
        } else if (p->x50 != 0) {
            n = p->x52;
            for (i = 0x20; i < 0x50; i++) {
                if (func_80034CD8(9, i)) {
                    if (--n < 0) {
                        break;
                    }
                }
            }
            func_80051BAC(i, 0x50, 6, p->x28);
            v = 6;
        } else {
            v = 9;
        }
        p->x2 = v;
    tail:
        p->x3 = 0;
        p->x4 = 0;
        p->x4F = 0;
        p->x52 = 0;
        p->x4E = 0;
        p->x94 = 0;
    }
    if (p->x5A != 0) {
        c3 = 0x80303030;
        c2 = 0x808080;
    } else {
        c3 = 0x808080;
        c2 = 0x80303030;
    }
    func_80050D80(D_80185408, 0x8B, 0x98, 0xC, 0x7C8F, c2, 2);
    func_80050D80(D_801850C8, 0xA8, 0x93, 0xC, 0x7C8E, c2, 3);
    func_80050D80(D_80184FE0, 0x87, 0x88, 0xC, 0x7C8F, c3, 2);
    func_80050D80(D_801850C8, 0xA8, 0x83, 0xC, 0x7C8E, c3, 3);
    if (++p->x88 >= 0x4F) {
        p->x88 = 0;
    }
    b.x = 0x86;
    b.y = p->x5A * 16 + 0x86;
    b.w = 0x44;
    b.h = 0xA;
    a = b;
    func_800514C4((void *)&a, (unsigned short)p->x88, 0x27, 2, 0xFFFF);
    c.x = 0x86;
    c.y = p->x5A * 16 + 0x86;
    c.w = 0x44;
    c.h = 0xA;
    b = c;
    func_800514C4((void *)&b, (unsigned short)(p->x88 + 0x27), 0x27, 2, 0xFFFF);
    func_8004C960(p);
    func_8004DFF4(p, 0x80, 0x18, 2);
    func_8004F9A4(0x10, 0x58);
    func_8004F710(0x80, 0x58, 0x808080);
    func_8004F450(p);
    func_80050D80(D_80184A40, 0x60, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    func_80050D80(D_80184B60, 0xE8, 0xC4, 0xD, 0x7C8E, 0x80808080, 3);
    if (p->x5A == 0) {
        func_800506BC(D_8008A82C, 0x20, 0xC5, 0x64, 0x808080, 0x64, 0);
    } else {
        func_800506BC(D_8008A854, 0x20, 0xC5, 0x64, 0x808080, 0x64, 0);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80055D5C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_80055F40);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_8004CE64", func_8005615C);

extern unsigned short D_800ABA90;
extern char D_8008A9B0[];
extern char D_8008A9E0[];
extern void func_80044834(void *, int);
extern void func_80050030();
extern int func_80050934();
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
extern void func_8004D028();


extern void func_80051054();
extern int func_80050A90(char *s, int x, int y, int z, int a, int b);




extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


