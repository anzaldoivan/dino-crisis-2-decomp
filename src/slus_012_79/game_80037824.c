#include "common.h"
#include "dc2.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;

void func_80037824(int a0, int a1)
{
    D_800893D0.a = a0;
    if (D_800ABA96 == 0) {
        a1 += 0x100;
    }
    D_800893D0.b = a1;
}

extern Pair D_800893D0;
struct Obj0x800379c0;
extern struct Obj0x800379c0 D_800B24A0;
typedef struct Obj0x8003784c {
    char pad0[0x10];
    int x10[2];
    int x18;
    char pad1c[0x20 - 0x1C];
    unsigned int x20;
    int x24;
    int x28;
    char pad2c[0x30 - 0x2C];
    int x30;
    int x34;
    char pad38[0x3C - 0x38];
    unsigned short x3C;
    char pad3e[0x44 - 0x3E];
    unsigned short x44;
    unsigned short x46;
} Obj0x8003784c;
extern int D_800B5DBC;
extern void func_80072484();
extern int func_8007B380(int);
extern int func_8007B62C();
extern void func_80085738();

void func_8003784C(void)
{
    Obj0x8003784c *p = (Obj0x8003784c *)&D_800B24A0;
    Pair *q;
    unsigned int i;

    if (p->x30 != 0 && D_800B5DBC != 0) {
        func_80072484();
        D_800B5DBC = 0;
    }
    i = p->x20;
    p->x46 += i;
    for (i = 0; i < p->x20; i++) {
        int v;
        q = &D_800893D0;
        func_8007B62C(q, p->x10[p->x18] + i * p->x24);
        v = D_800893D0.a;
        if (p->x30 != 0) {
            v += 0x18;
        } else {
            v += 0x10;
        }
        q->a = v;
        func_8007B380(0);
    }
    p->x18 ^= 1;
    if (p->x46 < p->x44) {
        func_80085738(p->x10[p->x18], p->x28 * p->x20);
    } else {
        p->x46 = 0;
        p->x34 = 0;
        D_800893D0.a = p->x3C;
        WORK->x4B9 = 1;
    }
}

typedef struct Obj0x800379c0 {
    char pad0[0x4C];
    unsigned short x4C;
} Obj0x800379c0;
typedef struct Rec0x800379c0 {
    char pad0[0x8];
    unsigned int x8;
} Rec0x800379c0;
extern Obj0x800379c0 D_800B24A0;
extern int func_800723A4(int *, Rec0x800379c0 **);

int func_800379C0(unsigned int arg0)
{
    int a;
    Rec0x800379c0 *b;

    if (++D_800B24A0.x4C >= 150) {
        WORK->x4BA = 2;
    }
    if (func_800723A4(&a, &b) != 0) {
        return 0;
    }
    D_800B24A0.x4C = 0;
    if (arg0 < b->x8) {
        WORK->x4BA = 2;
    }
    if (b->x8 < WORK->x4BC) {
        WORK->x4BA = 2;
    }
    WORK->x4BC = b->x8;
    return a;
}

typedef struct Obj0x80037aa0 {
    int x0;
    int x4[2];
    int xC;
    int x10[2];
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
    int x30;
    int x34;
    int x38;
} Obj0x80037aa0;
extern int func_800379C0();
extern void func_800722B4();
extern void func_800856BC();
extern void func_80085738();
extern void func_80085C24();

void func_80037AA0(Obj0x80037aa0 *p)
{
    int r;

    if (p->x38 != 0) {
        func_800856BC(p->x4[p->xC], p->x30);
        func_80085738(p->x10[p->x18], p->x28 * p->x20);
        p->x34 = 1;
        p->xC ^= 1;
    }
    r = func_800379C0(p->x2C);
    if (r == 0) {
        p->x38 = 0;
        return;
    }
    p->x38 = 1;
    func_80085C24(r, p->x4[p->xC]);
    func_800722B4(r);
}

typedef struct Obj0x80037b7c {
    char pad0[0x34];
    int x34;
    int x38;
} Obj0x80037b7c;
extern void func_80037AA0();

void func_80037B7C(Obj0x80037b7c *p)
{
    if (p->x34 == 0 || p->x38 == 0) {
        func_80037AA0(p);
    }
}

extern int func_80075148(int, int, int, int);
extern int func_80077244(int, int);
extern void func_800782E4(void);

void func_80037BBC(int arg0)
{
    unsigned char buf[2];

loop:
    buf[1] = 1;
    buf[0] = 1;
    func_800782E4();
    do {
    } while (func_80075148(0xD, (int)buf, 0, -1) == 0);
    if (func_80077244(arg0, 0x1C8) == 0) {
        goto loop;
    }
}

extern unsigned char D_800B24F8[];
extern void *func_80081CA4(void *, int, int);

void *func_80037C1C(void)
{
    void *r;

    r = func_80081CA4(D_800B24F8, 0, 0x14);
    WORK->x5DC = D_800B24F8;
    return r;
}

extern void func_80079904();

void func_80037C5C(void)
{
    unsigned char *p = WORK->x5DC;

    func_80079904(p[0], p[1], p[2]);
}

extern int D_800AF11C;
typedef struct Node0x80037c9c {
    char pad0[0x40];
    int x40;
    unsigned short x44;
    char pad46[0x54 - 0x46];
    struct Node0x80037c9c *next;
    char pad58[0x74 - 0x58];
    char x74[0xAC - 0x74];
    char xAC[4];
} Node0x80037c9c;
typedef struct {
    int a;
    short b;
} Rec0x80037c9c;
extern void func_80038354();

void func_80037C9C(void) {
    Unit *u;
    Node0x80037c9c *n;
    Rec0x80037c9c *r;
    int cnt;
    int f;
    unsigned char b;

    for (u = WORK->units; u < WORK->units + D_800AF11C; u++) {
        if (!(u->flags & 2)) {
            continue;
        }
        u->side ^= 1;
        f = u->side;
        b = u->xFE;
        if (b & 2) {
            u->xFE = b & ~2;
            f = 0;
        }
        n = (Node0x80037c9c *)u->id;
        cnt = 1;
        if (u->xFE & 1) {
            cnt = u->x68;
        }
        while (cnt-- != 0) {
            r = SPAD_ADDR_2C;
            r->a = n->x40;
            r->b = n->x44;
            func_80038354(WORK->x5DC, n->xAC, n->x74, ((unsigned char *)WORK->x318)[0x364 - 0x318] | (f << 8));
            n = n->next;
        }
    }
}

void func_80037E18(Unit *self)
{
    unsigned int cnt[2];
    Unit *u;

    cnt[1] = 0;
    cnt[0] = 0;
    for (u = WORK->units; u < WORK->units + D_800AF11C; u++) {
        if (u->flags & 1) {
            cnt[u->side]++;
        }
    }
    self->side = cnt[0] >= cnt[1];
}

void func_80037ED8(void)
{
    unsigned int cnt[2];
    Unit *u;

    u = WORK->units;
    cnt[1] = 0;
    cnt[0] = 0;
    cnt[u->side]++;
    for (u = WORK->units + 1; u < WORK->units + D_800AF11C; u++) {
        if (u->flags & 1) {
            u->side = cnt[0] > cnt[1];
            cnt[u->side]++;
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80037FD4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038354);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038568);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038780);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_8003889C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_80038930);

typedef struct {
    char pad0[0x3A];
    short x3A;
} Sub0x80038fb4;
typedef struct {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x4A - 0x46];
    short x4A;
    char pad4c[0x50 - 0x4C];
    int x50;
    int x54;
    char pad58[0x5A - 0x58];
    short x5A;
    char pad5c[0xE0 - 0x5C];
    short xE0;
    short xE2;
    short xE4;
    char pade6[0x108 - 0xE6];
    unsigned short x108;
    unsigned short x10A;
    char pad10c[0x118 - 0x10C];
    unsigned char x118;
    char pad119[0x128 - 0x119];
    void *x128;
    void *x12C;
    char pad130[0x134 - 0x130];
    void *x134;
    char pad138[0x15C - 0x138];
    unsigned char x15C;
    unsigned char x15D;
    unsigned char x15E;
    unsigned char x15F;
    unsigned char x160;
    char pad161[0x164 - 0x161];
    short x164;
    short x166;
    short x168;
    short x16A;
    short x16C;
    short x16E;
    char pad170[0x17B - 0x170];
    unsigned char x17B;
    char pad17c[0x17E - 0x17C];
    unsigned short x17E;
    char pad180[0x188 - 0x180];
    int x188;
} Obj0x80038fb4;
extern char D_800893E4[];
extern char D_800893F0[];
extern char D_800893FC[];
extern char D_80089408[];
extern Sub0x80038fb4 *D_800B25DC;
extern void func_8003E144();
extern void func_80042108();
extern void func_800428D0();
extern void func_8004A4D4();
extern void func_8004A970();
extern void func_8004AA70();
extern void func_8004AC08();

void func_80038FB4(Obj0x80038fb4 *obj)
{
    Work *w;
    unsigned short t;

    obj->x50 = 3;
    w = WORK;
    if (w->xC2B & 1) {
        t = w->xD98;
        obj->x10A = 1200;
        obj->x108 = t;
    } else {
        t = w->xD94;
        obj->x10A = 1200;
        obj->x108 = t;
    }
    obj->xE0 = obj->x40 = WORK->x3DC;
    obj->xE2 = obj->x42 = WORK->x3DE;
    obj->xE4 = obj->x44 = WORK->x3E0;
    obj->x4A = WORK->x3E2;
    obj->x17E = WORK->xD9C;
    obj->x17B = 0;
    obj->x12C = D_800893FC;
    obj->x134 = D_80089408;
    obj->x5A = obj->x42 / -1000;
    func_800428D0(obj);
    func_80042108(WORK->xC50 & 0xFF, WORK->xC50 >> 8);
    func_8003E144(obj);
    obj->x15F = 7;
    obj->x164 = 0x60;
    obj->x166 = 0x60;
    obj->x16A = 0x100;
    obj->x16E = 0x280;
    obj->x15D = 0xFF;
    obj->x160 = 0;
    obj->x15E = 0;
    obj->x168 = 0x200;
    obj->x16C = 0x300;
    obj->x15C = 0;
    if (WORK->xC2B == 2) {
        obj->x128 = D_800893F0;
        func_8004AA70(obj, 10, 13);
        obj->x15F = 0;
        func_8004A4D4(obj, 7, 0x1000, 0x1000);
        obj->x118 |= 0x20;
    } else {
        obj->x128 = D_800893E4;
        func_8004AA70(obj, 11, 14);
        func_8004A4D4(obj, 8, 0x1000, 0x1000);
    }
    obj->x188 = 0;
    D_800B25DC->x3A = 0x80;
    obj->x54 = 1;
    func_8004AC08(obj);
    func_8004A970(obj);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_80037824", func_800391E8);




