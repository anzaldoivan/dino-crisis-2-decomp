#include "common.h"
#include "dc2.h"
#include "../shared/func_8005A34C.h"
#include "../shared/func_800610A0.h"
#include "../shared/func_800631D8.h"
#include "../shared/func_8006600C.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;





extern void (*D_8008A1C8[])(void *);

void func_80059250(void *arg0)
{
    D_8008A1C8[WORK->x30E](arg0);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_80059294);

typedef struct Obj0x8005978c {
    unsigned char pad0[0x28];
    unsigned char state;
} Obj0x8005978c;
extern void (*D_8008A1D0[])(Obj0x8005978c *);

void func_8005978C(Obj0x8005978c *obj)
{
    D_8008A1D0[obj->state](obj);
}

typedef struct Obj0x800597c8 {
    unsigned char pad0[0x2];
    unsigned char x2;
    unsigned char x3;
    unsigned char pad4[0x28 - 0x4];
    unsigned char x28;
    unsigned char pad29[0x2B - 0x29];
    unsigned char x2B;
} Obj0x800597c8;
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80037068(int);
extern void func_800371A0(void);

void func_800597C8(Obj0x800597c8 *obj)
{
    switch (WORK->x310) {
    case 0:
        func_80034C48(3, 0, 0);
        WORK->x310++;
        func_80037068(obj->x3 + 1);
        break;
    case 1:
        func_800371A0();
        if (func_80034CD8(2, 0x17) == 0) {
            WORK->x310++;
        }
        break;
    case 2:
        obj->x2 = 0;
        obj->x2B = 0;
        WORK->x30E = 0;
        WORK->x310 = 0;
        obj->x28 ^= 1;
        break;
    }
}

/* Blk0x800598e0: dc2.h (T8 w36) */
typedef struct Q0x800598e0 {
    int v[4];
} Q0x800598e0;
typedef struct Rec0x800598e0 {
    short x0;
    short x2;
    char pad4[12];
} Rec0x800598e0;
typedef struct Hdr0x800598e0 {
    unsigned short x0;
    short x2;
    Blk0x800598e0 blk;
    Rec0x800598e0 recs[1];
} Hdr0x800598e0;
typedef struct Work0x800598e0 {
    char pad0[0x202];
    unsigned char x202;
    char pad203[1];
    short x204;
    char pad206[0x30E - 0x206];
    short x30E;
    short x310;
    char pad312[0x3B0 - 0x312];
    signed char x3B0;
    char pad3b1[0x3DC - 0x3B1];
    short x3DC;
    short x3DE;
    short x3E0;
    short x3E2;
    char pad3e4[0xC28 - 0x3E4];
    union {
        Blk0x800598e0 blk;
        struct {
            char pad[0x20];
            short xC48;
            short xC4A;
            short xC4C;
            short xC4E;
        } s;
    } xC28;
    char pad1088[0x1088 - 0xC28 - 0x460];
    union {
        Q0x800598e0 q;
        struct {
            char pad[4];
            unsigned char b;
        } s;
    } x1088;
} Work0x800598e0;
typedef struct Obj0x800598e0 {
    unsigned short x0;
    unsigned char x2;
    char pad3[0x28 - 3];
    unsigned char x28;
    unsigned char x29;
    char pad2a[1];
    unsigned char x2B;
    Rec0x800598e0 *x2C;
    Q0x800598e0 x30;
} Obj0x800598e0;
#define WORK0x800598e0 (*SCRATCH(Work0x800598e0 *, 0))
#define SPAD_080x800598e0 (*SCRATCH(unsigned short, 0x8))
#define SPAD_0A0x800598e0 (*SCRATCH(unsigned short, 0xA))
#define HDR0x800598e0 ((Hdr0x800598e0 *)D_8015F500)
#define SRC0x800598e0 ((Blk0x800598e0 *)D_8015F504)
extern unsigned short D_800ABA94;
extern unsigned short D_800AF132;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);
extern void func_8002EB14(int, int, int);
extern void func_8002F36C();
extern void func_8002F3E8();
extern void func_8002F5C8();
extern void func_8002F654();
extern void func_8002FBA8();
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80041BB8();

void func_800598E0(Obj0x800598e0 *o)
{
    Rec0x800598e0 r;
    int t;

    switch (WORK0x800598e0->x310) {
    case 0:
        WORK0x800598e0->x310++;
        func_8002F36C();
        o->x30 = WORK0x800598e0->x1088.q;
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        func_8001ECBC(o->x29 + 0x23, (int)HDR0x800598e0);
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        WORK0x800598e0->xC28.blk = HDR0x800598e0->blk;
        t = (int)HDR0x800598e0->recs;
        D_800ABA94 = HDR0x800598e0->x0;
        o->x2C = (Rec0x800598e0 *)t;
        func_80034C48(0, 10, 1);
        WORK0x800598e0->x1088.s.b = 1;
        WORK0x800598e0->x3DC = WORK0x800598e0->xC28.s.xC48;
        WORK0x800598e0->x3DE = WORK0x800598e0->xC28.s.xC4A;
        WORK0x800598e0->x3E0 = WORK0x800598e0->xC28.s.xC4C;
        WORK0x800598e0->x3E2 = WORK0x800598e0->xC28.s.xC4E;
        o->x0 = 0;
        WORK0x800598e0->x204 = 0;
        WORK0x800598e0->x202 = 1;
        func_8002F3E8();
        func_8002F5C8();
        break;
    case 1:
        WORK0x800598e0->x310++;
        t = HDR0x800598e0->x0;
        D_800ABA94 = t;
        func_8002F654();
        func_80034C48(2, 15, 1);
        break;
    case 2:
        o->x0++;
        r = *o->x2C;
        o->x2C++;
        SPAD_080x800598e0 = r.x0;
        SPAD_0A0x800598e0 = r.x2;
        func_8002FBA8();
        if (o->x0 >= 0x244) {
            func_8002EB14(1, 10, 1);
        } else if (o->x0 >= 0x15 && (D_800AF132 & 0x9F0)) {
            func_8002EB14(1, 10, 1);
        }
        if (WORK0x800598e0->x3B0 == 1 && !func_80034CD8(2, 6)) {
            WORK0x800598e0->x310++;
        }
        break;
    case 3:
        o->x2 = 0;
        func_80041BB8();
        o->x2B = 0;
        WORK0x800598e0->x30E = 0;
        WORK0x800598e0->x310 = 0;
        o->x28 ^= 1;
        o->x29 = (o->x29 + 1) & 3;
        WORK0x800598e0->x1088.q = o->x30;
        func_80034C48(0, 10, 0);
        break;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_80059CB4);

typedef struct Obj0x80059fb0 {
    char pad0[0x2B];
    unsigned char x2B;
} Obj0x80059fb0;
extern void func_8002EB14(int, int, int);
extern int func_80034CD8(int, int);
extern void func_8004262C(int);

void func_80059FB0(Obj0x80059fb0 *obj)
{
    short s;

    s = WORK->x312;
    switch (s) {
    case 0:
        WORK->x312++;
        func_8002EB14(1, 10, 1);
        break;
    case 1:
        if (func_80034CD8(2, 6) == 0) {
            WORK->x21A = 0xFFFF;
            obj->x2B = s;
            WORK->x30E = 0;
            WORK->x310 = 0;
            WORK->x312 = 0;
            func_8004262C(2);
        }
        break;
    }
}

typedef struct Obj0x8005a07c {
    char pad0[0x2B];
    unsigned char x2B;
} Obj0x8005a07c;
extern void func_8002EB14(int, int, int);
extern int func_80034CD8(int, int);
extern void func_8004262C(int);

void func_8005A07C(Obj0x8005a07c *obj)
{
    short s;

    s = WORK->x312;
    switch (s) {
    case 0:
        WORK->x312++;
        func_8002EB14(1, 10, 1);
        break;
    case 1:
        if (func_80034CD8(2, 6) == 0) {
            WORK->x6E8 = 0;
            obj->x2B = s;
            WORK->x30E = 0;
            WORK->x310 = 0;
            WORK->x312 = 0;
            func_8004262C(0);
        }
        break;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005A144);

void func_8005A34C(Tbl_s8005A34C *t, int i, unsigned char *p)
{
#include "../shared/func_8005A34C.inc.c"
}

typedef struct Node0x8005a37c {
    char pad0[4];
    unsigned char x4;
    unsigned char x5;
    char pad6[2];
} Node0x8005a37c;
typedef struct Elem0x8005a37c {
    int x0;
    Node0x8005a37c *x4;
    unsigned char x8;
    char pad9[3];
} Elem0x8005a37c;
typedef struct Obj0x8005a37c {
    int x0;
    Elem0x8005a37c x4[1];
} Obj0x8005a37c;

int func_8005A37C(Obj0x8005a37c *o, int i)
{
    Elem0x8005a37c *e = &o->x4[i];
    Node0x8005a37c *n;
    int r = 0;

    if (e->x8 != 0) {
        e->x8--;
    } else {
        n = e->x4;
        if (!((n->x4 >> 5) & 1)) {
            n += n->x4 & 0x1F;
            e->x4 = n;
            if ((e->x8 = n->x5) == 1) {
                r = (signed char)n->x4 >> 5;
            }
        }
    }
    return r;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005A400);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005A580);

extern void func_8005A580(void);
extern int func_80034CD8(int, int);
extern void func_80029F7C(void);
extern void func_8005AF1C(void);
extern void func_8001D4AC();
extern void func_8005B154(void);
extern void func_8005BFC8(void);
extern void func_8005B6B0(void);
extern void func_8005B928(void);
extern void func_800654F8(void);
extern void func_8005AE5C(void);
extern void func_8002AE2C(void);
extern void func_8007E0B4();
extern unsigned int *D_800ABA98;
extern unsigned char D_800ABA96;
extern char D_800AD06C[];

void func_8005AD28(void)
{
    func_8005A580();
    if (func_80034CD8(2, 0x12)) {
        func_80029F7C();
    }
    func_8005AF1C();
    if (func_80034CD8(3, 5)) {
        func_8001D4AC(WORK->x318);
    }
    if (func_80034CD8(3, 1)) {
        func_8005B154();
    }
    if (func_80034CD8(3, 3)) {
        func_8005BFC8();
    }
    if (func_80034CD8(3, 2)) {
        func_8005B6B0();
    }
    if (func_80034CD8(3, 4)) {
        func_8005B928();
    }
    func_800654F8();
    if (func_80034CD8(3, 8)) {
        func_8005AE5C();
    }
    func_8002AE2C();
    func_8007E0B4(D_800ABA98 + 0x22, D_800AD06C + (D_800ABA96 << 12),
                  D_800AD06C + (D_800ABA96 << 12) - 0xFFC);
}

typedef struct Ent0x8005ae5c {
    unsigned char x0;
    char pad1[0x4 - 0x1];
    void (*x4)(struct Ent0x8005ae5c *);
    char pad8[0x34 - 0x8];
} Ent0x8005ae5c;
extern int D_800AF5B8;

void func_8005AE5C(void)
{
    Ent0x8005ae5c *p;

    for (p = (Ent0x8005ae5c *)WORK->x4E8; p < &((Ent0x8005ae5c *)WORK->x4E8)[D_800AF5B8]; p++) {
        if ((p->x0 & 1) && p->x4 != 0) {
            p->x4(p);
        }
    }
}

typedef struct Tile0x8005af1c {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, w, h;
} Tile0x8005af1c;
extern unsigned int *D_800ABA98;
extern void func_8007E114();
extern int func_8007E034();
extern void func_8007E194();

void func_8005AF1C(void)
{
    int c = WORK->x6B0;
    unsigned int *ot;

    if (c == 0)
        return;

    func_8007E114(PKT);
    ((Tile0x8005af1c *)PKT)->code |= 2;
    ((Tile0x8005af1c *)PKT)->x0 = 0;
    ((Tile0x8005af1c *)PKT)->y0 = 0;
    ((Tile0x8005af1c *)PKT)->w = 0x140;
    ((Tile0x8005af1c *)PKT)->h = 0x1C;
    ((Tile0x8005af1c *)PKT)->r0 = ((Tile0x8005af1c *)PKT)->g0 = ((Tile0x8005af1c *)PKT)->b0 = c;
    ot = D_800ABA98 + 0x1E;
    *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT += 4;

    func_8007E114(PKT);
    ((Tile0x8005af1c *)PKT)->code |= 2;
    ((Tile0x8005af1c *)PKT)->x0 = 0;
    ((Tile0x8005af1c *)PKT)->y0 = 0;
    ((Tile0x8005af1c *)PKT)->w = 0x140;
    ((Tile0x8005af1c *)PKT)->h = 0x1C;
    ((Tile0x8005af1c *)PKT)->y0 = 0xD4;
    ((Tile0x8005af1c *)PKT)->r0 = ((Tile0x8005af1c *)PKT)->g0 = ((Tile0x8005af1c *)PKT)->b0 = c;
    ot = D_800ABA98 + 0x1E;
    *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT += 4;

    func_8007E194(PKT, 1, 0, func_8007E034(0, 2, 0x140, 0x100), 0);
    ot = D_800ABA98 + 0x1E;
    *PKT = (*PKT & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
    PKT += 3;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005B154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005B6B0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005B928);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005BFC8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005C608);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005CB10);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005CBE8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005CC04);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005CE28);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D0CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D154);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D4BC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D72C);

int func_8005D930(void) {
    return SPAD_60;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D940);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D97C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005D9C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005DA04);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005DA50);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005DCE0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005DEF0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005E160);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80059250", func_8005E1DC);

extern void func_80018020(void *, int);








extern void func_80034C48(int, int, int);






#define SHARED_F0 func_80034D88

#define SHARED_F0 func_8002EB14


extern void (*D_8008A3B0[])(Elem28 *);


#define SHARED_F0 func_80041994


#define SHARED_F0 func_80030DDC


#define SHARED_F0 func_80044AB4

#define SHARED_F0 func_80044B80

extern int func_80034CD8(int, int);


extern int func_80034CD8(int, int);
extern void func_8001D9E4(void);
extern void func_8001E4A0(int, int);


extern unsigned short *func_800458F0(int a, int b);


extern Elem28 D_800B4A68[];


extern void func_80034C48(int, int, int);
extern unsigned char D_800B49D4;


extern int func_80034CD8(int, int);
extern int func_80065D0C(int, int, char *, int);
extern char D_8008EB88[];
extern char D_8008E98E[];


extern int D_800B4F74;



extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;




#define SHARED_D0 D_8008F780




#define SHARED_D0 D_8008F78C

#define SHARED_F0 func_800661FC

