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













extern void func_80034C48(int, int, int);






#define SHARED_F0 func_80034D88

#define SHARED_F0 func_8002EB14


extern void (*D_8008A3B0[])(Elem28 *);


#define SHARED_F0 func_80041994


#define SHARED_F0 func_80030DDC

extern char *func_800474B4(int a, int b);



#define SHARED_F0 func_80044AB4

#define SHARED_F0 func_80044B80

extern int func_80034CD8(int, int);


extern int func_80034CD8(int, int);
extern void func_8001D9E4(void);
extern void func_8001E4A0(int, int);


extern unsigned short *func_800458F0(int a, int b);


extern Elem28 D_800B4A68[];


extern void func_80018020(void *, int);
extern void func_80034C48(int, int, int);
extern void func_80063D78(void);


typedef struct Obj0x800644cc {
    char pad0[0x14];
    unsigned char x14;
    unsigned char x15;
    unsigned char x16;
} Obj0x800644cc;
extern void func_80034C48(int, int, int);

void func_800644CC(Obj0x800644cc *obj)
{
    func_80034C48(obj->x14, obj->x15, obj->x16);
}

extern void func_80051F8C(int, int, int, int, int);

void func_800644FC(Elem28 *e)
{
    func_80051F8C(1, e->x14, e->x1A, e->x18, (unsigned short)((e->x1B << 8) | (e - WORK->x5FC)));
}

typedef struct Obj0x80064578 {
    char pad0[0x14];
    int x14;
} Obj0x80064578;
extern void func_80034C48(int, int, int);

void func_80064578(Obj0x80064578 *obj)
{
    func_80034C48(1, 0, 1);
    WORK->x4DC[obj->x14].x4E = 2;
    WORK->x4DC[obj->x14].x46 = 1;
}

extern void func_80034C48(int, int, int);
extern unsigned char D_800B49D4;

void func_800645F4(void)
{
    func_80034C48(1, 3, 1);
    D_800B49D4 = 3;
}

extern void func_80051F8C(int, int, int, int, int);

void func_80064624(void) {
    func_80051F8C(2, 0, 0, 0, 0);
}

typedef struct Obj0x80064654 {
    char pad0[0x14];
    short f14;
    char pad16[2];
    signed char f18;
    unsigned char f19;
} Obj0x80064654;
extern void func_80034C48(int, int, int);
extern void func_8005E854(int, int);

void func_80064654(Obj0x80064654 *arg0)
{
    if (WORK->x6EA == arg0->f19) {
        WORK->x6EA = 0xFF;
        func_8005E854(arg0->f18, arg0->f14);
    }
    func_80034C48(10, arg0->f19, 1);
}

typedef struct Obj0x800646b8 {
    unsigned char x0;
    char pad1[0x14 - 0x1];
    unsigned char x14;
    unsigned char x15;
    unsigned char x16;
    unsigned char x17;
    int x18;
} Obj0x800646b8;
typedef struct Ent0x800646b8 {
    char pad0[0x54];
    unsigned char x54;
    char pad55[0x64 - 0x55];
    unsigned char x64;
    unsigned char x65;
    unsigned char x66;
    char pad67[0xAC - 0x67];
} Ent0x800646b8;
typedef struct Work0x800646b8 {
    char pad0[0x4E0];
    Sub0x80026a7c *units;
    Ent0x800646b8 *x4E4;
    char pad4e8[0xC50 - 0x4E8];
    unsigned short xC50;
} Work0x800646b8;
#define WORK0x800646b8 (*SCRATCH(Work0x800646b8 *, 0))
typedef struct Fx0x800646b8 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x4C - 0x46];
    short x4C;
    unsigned char x4E;
    unsigned char x4F;
    int x50;
    char pad54[0x68 - 0x54];
    int x68;
} Fx0x800646b8;
typedef struct Tbl0x800646b8 {
    unsigned short a;
    unsigned short b;
    unsigned char c;
    char pad5;
} Tbl0x800646b8;
extern void func_80034C48(int, int, int);
extern void func_80044834(void *, int);
extern Fx0x800646b8 *func_800478B8(void);
extern unsigned short D_800B4F68[3];
extern Tbl0x800646b8 D_8008A3E0[][8];

void func_800646B8(Obj0x800646b8 *arg0)
{
    unsigned int i;
    Tbl0x800646b8 *t;
    Fx0x800646b8 *p;
    int mode;

    mode = WORK0x800646b8->xC50 >> 8;
    if (mode == 1 || mode == 7) {
        if (arg0->x14 == 2) {
            func_80044834(WORK0x800646b8->units, 0x26);
            t = D_8008A3E0[3 - arg0->x16];
            for (i = 0; i < 8; i++, t++) {
                p = func_800478B8();
                if (p != 0) {
                    p->x4E = 8;
                    p->x50 |= 7;
                    p->x68 = arg0->x18;
                    p->x40 = D_800B4F68[0];
                    p->x42 = D_800B4F68[1] + t->a;
                    p->x44 = D_800B4F68[2];
                    p->x4C = (WORK0x800646b8->units->x4A + t->b) & 0xFFF;
                    p->x4F = t->c;
                }
            }
            switch (--arg0->x16) {
            case 0:
                func_80034C48(8, arg0->x15, 1);
                arg0->x0 = 0;
                WORK0x800646b8->x4E4[arg0->x17].x54 = 3;
                break;
            case 2:
                WORK0x800646b8->x4E4[arg0->x17].x54 = 2;
                break;
            }
        }
    } else if (mode == 2 || mode == 8) {
        if (arg0->x14 == 1) {
            Ent0x800646b8 *e = &WORK0x800646b8->x4E4[arg0->x18];
            e->x64 = 0;
            e->x65 = 0xFF;
            e->x66 = 0;
            func_80044834(WORK0x800646b8->units, 0x27);
            func_80034C48(8, arg0->x15, 1);
            arg0->x0 = 0;
        }
    }
}

extern int func_80034CD8(int, int);
extern int func_80065D0C(int, int, char *, int);
extern char D_8008EB88[];
extern char D_8008E98E[];

void func_8006493C(unsigned char *arg0)
{
    if (func_80034CD8(8, arg0[0x14])) {
        func_80065D0C(0x1E, 0xBE, D_8008EB88, 3);
    } else {
        func_80065D0C(0x1E, 0xBE, D_8008E98E, 3);
    }
}

extern int D_800B4F74;

int func_80064998(void) {
    return D_800B4F74;
}

extern unsigned short D_800B4F70;

void func_800649A8(int arg0, int arg1, int arg2) {
    Work *w = WORK;
    short y = arg1;
    int mode = w->xC50 >> 8;

    D_800B4F68[0] = arg0;
    D_800B4F68[1] = y;
    D_800B4F68[2] = arg2;
    if (*(unsigned char *)&w->xC50 == 11) {
        D_800B4F70 = 0x30;
        D_800B4F68[1] = y + 700;
    } else if (mode == 1 || mode == 7) {
        D_800B4F70 = 0x10;
    } else if (mode == 2 || mode == 8) {
        D_800B4F70 = 0x20;
    }
    func_80034C48(2, 10, 1);
}

extern void func_80065EC8(void);
extern void func_80018020(void *, int);
extern void func_80065F24(int, int);
extern void func_80065EFC(void);
extern void func_80034C48(int, int, int);
extern S800B53A8 D_800B53A8;

void func_80064A60(void)
{
    func_80065EC8();
    func_80018020(&D_800B53A8, 7);
    func_80065F24(0x19, 0xB0);
    func_80065EFC();
    func_80034C48(2, 0x1F, 0);
}

typedef struct Ent0x80064ab4 {
    unsigned char flags;
    unsigned char kind;
    unsigned char pad2[0xE];
} Ent0x80064ab4;
extern Ent0x80064ab4 D_800B4F78[32];
extern void (*D_8008A560[])(Ent0x80064ab4 *);
extern void (*D_8008A57C[])(void);

void func_80064AB4(void)
{
    Ent0x80064ab4 *e;

    for (e = D_800B4F78; e < &D_800B4F78[32]; e++) {
        if (e->flags & 1) {
            D_8008A560[e->kind](e);
        }
    }
    if (D_800B53A8.pad0[0] & 1) {
        D_8008A57C[D_800B53A8.unk2]();
    }
}

typedef struct Ent0x80064b88 {
    unsigned char flags;
    unsigned char kind;
    unsigned char count;
} Ent0x80064b88;

void func_80064B88(Ent0x80064b88 *e)
{
    if (++e->count == 2) {
        e->flags = 0;
    }
}

void func_80064BB0(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800644CC", func_80064BB8);

extern S800B53A8 D_800B53A8;

void func_80064DCC(void)
{
    S800B53A8 *p = &D_800B53A8;
    unsigned short *t = &p->x0C;

    if (*t != 0) {
        if (--*t == 0) {
            p->unkF = 0;
            p->unk2--;
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800644CC", func_80064E0C);

extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;

void func_80065004(void)
{
    S800B53A8 *p = &D_800B53A8;
    if (!(D_800ABA68 & 0xA0)) {
        p->unkF = 0;
        p->unk2 -= 3;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800644CC", func_80065038);

typedef struct Obj0x800652dc {
    unsigned char pad0[2];
    unsigned char unk2;
    unsigned char pad3[9];
    unsigned short hC;
    unsigned char padE;
    unsigned char unkF;
} Obj0x800652dc;
extern unsigned short D_800ABA90;
extern S800B53A8 D_800B53A8;
extern char D_800B5178[];
extern void func_80044834(void *, int);

void func_800652DC(void)
{
    Obj0x800652dc *p = (Obj0x800652dc *)&D_800B53A8;
    unsigned char i;
    unsigned int v;

    if (D_800ABA90 & 0x8000) {
        for (i = 0; i < 0x8C; i++) {
            v = ((unsigned int *)D_800B5178)[i];
            if ((v >> 24) == 2 && (v & 0xFF) == p->unkF - 1) {
                func_80044834(0, 0);
                p->hC = 0;
                p->unkF--;
                break;
            }
        }
    } else if (D_800ABA90 & 0x2000) {
        for (i = 0; i < 0x8C; i++) {
            v = ((unsigned int *)D_800B5178)[i];
            if ((v >> 24) == 2 && (v & 0xFF) == p->unkF + 1) {
                func_80044834(0, 0);
                p->hC = 0;
                p->unkF++;
                break;
            }
        }
    }
    if (++p->hC > 0x14) {
        p->hC = 0;
    }
    if (D_800ABA90 & 0xA0) {
        p->unk2 -= 5;
    }
}

extern S800B53A8 D_800B53A8;
extern unsigned char D_800B49D5;
extern void func_80065EFC(void);
extern void func_80034C48(int, int, int);

void func_80065444(void)
{
    S800B53A8 *p = &D_800B53A8;

    D_800B49D5 = p->unkF;
    func_80065EFC();
    func_80034C48(2, 0x1F, 0);
    p->pad0[0] = 0;
}

typedef struct {
    unsigned char pad0[1];
    unsigned char pad1[0x14 - 1];
    int x14;
    unsigned char b18;
} S0x8006548c;
typedef struct {
    char pad0[0x6B4];
    int x6B4;
} Work0x8006548c;
#define WORK0x8006548c (*SCRATCH(Work0x8006548c *, 0))
extern S800B53A8 D_800B53A8;
extern void func_80034C48(int, int, int);
extern void func_80065EFC(void);

void func_8006548C(void)
{
    S0x8006548c *p;

    func_80065EFC();
    p = &D_800B53A8;
    D_800B53A8.pad0[0] = 0;
    WORK0x8006548c->x6B4 = p->x14;
    func_80034C48(2, 0x1F, 0);
    if (p->b18 & 1) {
        func_80034C48(2, 0xC, 1);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800644CC", func_800654F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800644CC", func_80065D0C);

void func_80065EC0(unsigned char *p)
{
#include "../shared/func_80047584.inc.c"
}

extern Ent0x80064ab4 D_800B4F78[32];
extern S800B53A8 D_800B53A8;

void func_80065EC8(void) {
    Ent0x80064ab4 *p;

    for (p = D_800B4F78; p < &D_800B4F78[32]; p++) {
        p->flags = 0;
    }
    D_800B53A8.pad0[0] = 0;
}

extern void func_80018020(void *, int);
extern char D_800B5178[];

void func_80065EFC(void)
{
    func_80018020(D_800B5178, 0x8C);
}

extern S800B53A8 D_800B53A8;

void func_80065F24(int a, int b)
{
    S800B53A8 *p = &D_800B53A8;
    p->b10 = (unsigned int)a >> 1;
    p->b11 = (unsigned int)b >> 1;
}

void func_80065F40(void)
{
#include "../shared/func_8001B530.inc.c"
}

typedef struct Obj0x80065f48 {
    char pad0[0x20];
    char mat[0x20];
    SVECTOR pos;
    SVECTOR rot;
    char pad50[4];
    struct Obj0x80065f48 *parent;
    char pad58[8];
    SVECTOR v60;
} Obj0x80065f48;
extern SVECTOR *func_80079704();

void func_80065F48(Obj0x80065f48 *obj)
{
    SVECTOR tmp;
    Obj0x80065f48 *p;

    if (obj->parent != 0) {
        func_80079704(obj->parent->mat, &obj->pos, &tmp);
        p = obj->parent;
        obj->pos.vx = p->pos.vx + tmp.vx;
        obj->pos.vy = p->pos.vy + tmp.vy;
        obj->pos.vz = p->pos.vz + tmp.vz;
        obj->rot.vx += p->rot.vx;
        obj->rot.vy += p->rot.vy;
        obj->rot.vz += p->rot.vz;
        func_80079704(p->mat, &obj->v60, &obj->v60);
        obj->parent = 0;
    }
}

#define SHARED_D0 D_8008F780
void func_8006600C(Obj8006600C_s8006600C *obj)
{
#include "../shared/func_8006600C.inc.c"
}

void func_80066048(unsigned char *a0)
{
    *(unsigned short *)(a0 + 0x4A) += 0x30;
}

void func_8006605C(void)
{
#include "../shared/func_8001B530.inc.c"
}

void func_80066064(void)
{
#include "../shared/func_8001B530.inc.c"
}

void func_8006606C(void)
{
#include "../shared/func_8001B530.inc.c"
}

#define SHARED_D0 D_8008F78C
void func_80066074(Obj8006600C_s8006600C *obj)
{
#include "../shared/func_8006600C.inc.c"
}

typedef struct ElA0x800660b0 {
    char pad0[0x1A];
    unsigned short f1A;
    char pad1C[0x28 - 0x1C];
} ElA0x800660b0;
typedef struct ElB0x800660b0 {
    char pad0[0x22];
    unsigned short f22;
    char pad24[0x34 - 0x24];
} ElB0x800660b0;
typedef struct Hdr0x800660b0 {
    ElA0x800660b0 *a;
    ElB0x800660b0 *b;
    unsigned short na;
    unsigned short nb;
} Hdr0x800660b0;
typedef struct Obj0x800660b0 {
    char pad0[0x46];
    unsigned char f46;
    signed char f47;
    char pad48[0x4F - 0x48];
    signed char f4F;
    char pad50[0x68 - 0x50];
    Hdr0x800660b0 *hdr;
} Obj0x800660b0;
extern void func_800661FC(void *arg0, int arg1);

void func_800660B0(Obj0x800660b0 *obj)
{
    Hdr0x800660b0 *h = obj->hdr;
    ElA0x800660b0 *a = h->a;
    ElB0x800660b0 *b = h->b;
    unsigned int i;

    for (i = 0; i < h->na; i++) {
        a->f1A += obj->f47;
        a++;
    }
    for (i = 0; i < h->nb; i++) {
        b->f22 += obj->f47;
        b++;
    }
    if (obj->f4F == 0) {
        obj->f4F = 8;
    }
    obj->f4F++;
    func_800661FC(obj, 0);
    obj->f46 = 1;
}

#define SHARED_F0 func_800661FC
void func_80066190(unsigned char *arg0)
{
#include "../shared/func_80066190.inc.c"
}

extern void func_800661FC(void *arg0, int arg1);

void func_800661C0(unsigned char *arg0)
{
    func_800661FC(arg0, (unsigned char)(arg0[0x4F] << 1));
    arg0[0x46] = 1;
}

