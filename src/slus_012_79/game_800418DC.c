#include "common.h"
#include "dc2.h"
#include "../shared/func_800436C8.h"
#include "../shared/func_8002B3E0.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;




extern unsigned char D_800AE547;




#define SHARED_D0 D_800894C8

#define SHARED_F0 func_800487B8

extern void (*D_800899E0[])(void *);
extern void (*D_80089A30[])(void *);


extern void (*D_80089AB8[])(void *);



extern Pair D_800B2890[];
void func_80018020(void *, int);
unsigned int func_8003252C(void *);
void func_800339AC(void *, Seg *, SVECTOR *);
int func_80033D4C(void *, void *, SVECTOR *, int);
int func_8003DD34(int, int, unsigned short);
int func_8003F154(void *, void *, int);
void func_80044834(void *, int);
char *func_800474B4(int a, int b);
void func_80047958(void *);
void func_80048048(void *, void *, int, int, int, int);
void func_8004A240(void *, void *, SVECTOR *);
int func_8004A32C(void);
void func_800649A8(int, int, int);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007AA34(SVECTOR *, MATRIX *);


extern int D_800181D4;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);


extern unsigned short *D_80089CE4[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);

void func_800418DC(unsigned short id)
{
    unsigned short v;

    v = D_80089CE4[id >> 8][id & 0xFF];
    if (v != WORK->x218) {
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        func_8001EC10(v);
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        WORK->x218 = v;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80041994);

typedef struct Ent0x80041a68 {
    unsigned short id;
    unsigned short pad;
} Ent0x80041a68;
extern Ent0x80041a68 D_8008A02C[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);

void func_80041A68(int idx)
{
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001EC10(D_8008A02C[idx].id);
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80041AE4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80041BB8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80041C30);

typedef struct Snd0x80041dc0 {
    unsigned short id;
    char pad2[6];
} Snd0x80041dc0;
extern Snd0x80041dc0 D_800187AC[];
extern Snd0x80041dc0 D_80018824[];
extern int D_800193BC[];
extern int D_800193E4[];
extern int D_80019408[];
extern int D_800ABA70;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);
extern void func_80034C48(int, int, int);

void func_80041DC0(unsigned int a0, unsigned int a1)
{
    Snd0x80041dc0 *p;
    int v;

    if (a0 - 2 >= 14) {
        if (a0 < 29) {
            return;
        }
        a0 -= 29;
        p = &D_80018824[a0];
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        func_8001EC10(p->id);
        while (func_8001D9D4()) {
            func_8001CDE4(1);
        }
        WORK->x220 = a0;
        WORK->x222 = a1;
        return;
    }

    func_80034C48(2, 0x16, 1);
    if (a1 & 0x40000000) {
        a1 = WORK->x222;
    } else {
        a1 &= 0xF;
        if (a0 == 2) {
            if (WORK->xF26 != 0) {
                a1 = 9;
            } else {
                a1 += WORK->xF24 * 3;
            }
        } else if (a0 == 7) {
            a1 += WORK->xF24 * 3;
        } else if (a0 != 6) {
            a1 = 0;
        }
    }

    if (a0 != 0) {
        p = &D_800187AC[a0];
        if (a1 == WORK->x222 && a0 == WORK->x220) {
            func_80034C48(2, 0x16, 0);
            return;
        }
        if (a0 != WORK->x220) {
            WORK->x222 = 0xFFFF;
            while (func_8001D9D4()) {
                func_8001CDE4(1);
            }
            func_8001EC10(p->id);
            while (func_8001D9D4()) {
                func_8001CDE4(1);
            }
        }
        if (a1 != WORK->x222) {
            v = -1;
            if (a0 == 2) {
                if (a1 >= 10) {
                    a1 = 9;
                }
                v = D_800193BC[a1];
            } else if (a0 == 7) {
                if (a1 >= 9) {
                    a1 = 8;
                }
                v = D_800193E4[a1];
            } else if (a0 == 6) {
                if (a1 >= 2) {
                    a1 = 1;
                }
                v = D_80019408[a1];
            }
            if (v != -1) {
                D_800ABA70++;
                while (func_8001D9D4()) {
                    func_8001CDE4(1);
                }
                func_8001EC10((unsigned short)v);
                while (func_8001D9D4()) {
                    func_8001CDE4(1);
                }
            }
        }
        WORK->x220 = a0;
        WORK->x222 = a1;
    }
    func_80034C48(2, 0x16, 0);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80042108);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_8004262C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80042758);

typedef struct Ent0x8004283c {
    unsigned short id;
    short pad2;
} Ent0x8004283c;
typedef struct Work0x8004283c {
    char pad0[0x8];
    void *x8;
} Work0x8004283c;
#define WORK0x8004283c (*SCRATCH(Work0x8004283c *, 0))
extern Ent0x8004283c D_800195C4[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);
extern void func_80034C48(int, int, int);

void func_8004283C(int idx)
{
    Ent0x8004283c *e;

    e = &D_800195C4[idx];
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001ECBC(e->id, (int)D_801C1500);
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_80034C48(3, 0, 1);
    WORK0x8004283c->x8 = D_801C1500;
}

void func_800428D0(void)
{
#include "../shared/func_8001B530.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_800428D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80042AE4);

typedef struct Pos0x80042d3c {
    short x;
    unsigned short y;
    short z;
} Pos0x80042d3c;
typedef struct Elem0x80042d3c {
    char pad0[0x8];
    unsigned short x8;
    unsigned short xA;
    unsigned short xC;
    short xE;
    char pad10[0x14 - 0x10];
} Elem0x80042d3c;
typedef struct Graph0x80042d3c {
    short n;
    short pad2;
    Elem0x80042d3c e[1];
} Graph0x80042d3c;
typedef struct Obj0x80042d3c {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x5A - 0x46];
    short x5A;
    char pad5c[0xD8 - 0x5C];
    unsigned short xD8;
    unsigned short xDA;
    unsigned short xDC;
    char padde[0x138 - 0xDE];
    short x138;
} Obj0x80042d3c;
typedef struct Act0x80042d3c {
    char pad0[0x6C];
    int x6C;
    char pad70[0x13C - 0x70];
    unsigned int x13C;
} Act0x80042d3c;
extern Obj0x80042d3c D_800B28C0;
extern char D_800B2B20[];
extern void func_80042E84();
extern void func_80043138();
extern void func_80042AE4();

int func_80042D3C(Act0x80042d3c *act, Pos0x80042d3c *pos)
{
    Obj0x80042d3c *o;
    Elem0x80042d3c *tbl;
    unsigned short y;

    tbl = (*(Graph0x80042d3c **)WORK->pad5f0)->e;
    o = &D_800B28C0;
    o->x40 = pos->x;
    y = pos->y;
    o->x42 = y;
    o->x44 = pos->z;
    o->x5A = (short)y / -1000;
    func_80042E84(o);
    o->xD8 = tbl[o->x138].x8;
    o->xDA = tbl[o->x138].xA;
    o->xDC = tbl[o->x138].xC;
    if (tbl[o->x138].xE == -0x8000) {
        o->xDA = o->x42;
    }
    func_80043138(o, D_800B2B20);
    func_80042AE4(act, o);
    if (*(unsigned short *)&WORK->xC2C == 0x903) {
        act->x13C = (act->x13C << 4) / (act->x6C >> 8);
    }
    return 0;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80042E84);

typedef struct Node0x80043138 {
    unsigned short done;
    unsigned short prev;
    unsigned int dist;
} Node0x80043138;
typedef struct Edge0x80043138 {
    int x0;
    unsigned int mask;
    char pad8[0x10 - 0x8];
    unsigned short costOfs;
    short pad12;
} Edge0x80043138;
typedef struct Graph0x80043138 {
    short n;
    short pad2;
    Edge0x80043138 e[1];
} Graph0x80043138;
typedef struct Obj0x80043138 {
    char pad0[0x138];
    short start;
} Obj0x80043138;

void func_80043138(Obj0x80043138 *obj, Node0x80043138 *nodes)
{
    int i;
    Graph0x80043138 *g;
    Node0x80043138 *p;
    Edge0x80043138 *edges;
    Edge0x80043138 *e;
    unsigned short *cost;
    unsigned int min;
    unsigned int d;
    int best;
    int cnt;

    best = 0;
    p = nodes + 16;
    g = *(Graph0x80043138 **)((char *)WORK + 0x5F0);
    edges = g->e;
    cnt = g->n;
    do {
        p--;
        p->done = 0;
        p->prev = 0;
        p->dist = 0x7FFFFFFF;
    } while (p >= nodes);
    nodes[obj->start].dist = 0;
    do {
        min = 0x7FFFFFFF;
        i = g->n;
        do {
            i--;
            if (nodes[i].done == 0 && nodes[i].dist < min) {
                min = nodes[i].dist;
                best = i;
            }
        } while (i > 0);
        cnt--;
        nodes[best].done = 1;
        e = &edges[best];
        cost = (unsigned short *)(e->costOfs + (int)g);
        i = g->n;
        do {
            i--;
            if (e->mask & (1 << i)) {
                if (nodes[i].done == 0) {
                    d = nodes[best].dist + *cost;
                    if (d < nodes[i].dist) {
                        nodes[i].dist = d;
                        nodes[i].prev = best;
                    }
                }
                cost++;
            }
        } while (i > 0);
    } while (cnt != 0);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043284);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_800434DC);

int func_800436C8(V3s_s800436C8 *a, V3s_s800436C8 *b)
{
#include "../shared/func_800436C8.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043714);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_800437F8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043884);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043958);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043A2C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043A7C);

extern void func_80045C18(int arg0);

void func_80043B24(void)
{
    unsigned char i;

    for (i = 0; i < 4; i++) {
        func_80045C18(i);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043B60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043BF8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043C8C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043CC4);

typedef struct Obj0x80043d94 {
    unsigned char pad0[0xF];
    signed char count;
} Obj0x80043d94;
extern Obj0x80043d94 D_800B32F0;
extern void func_80043E1C(int);

void func_80043D94(int dir)
{
    Obj0x80043d94 *o = &D_800B32F0;
    int i;

    if (dir != 0) {
        for (i = 0; i <= o->count; i++) {
            func_80043E1C(i);
        }
    } else {
        for (i = o->count; i >= 0; i--) {
            func_80043E1C(i);
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043E1C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043E74);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043F54);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80044078);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_800441A0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80044220);

typedef struct Rec0x800442d4 {
    signed char b0;
    signed char b1;
    signed char b2;
    signed char b3;
    char x4[0x10];
} Rec0x800442d4;
typedef struct Tbl0x800442d4 {
    unsigned int count;
    Rec0x800442d4 recs[1];
} Tbl0x800442d4;
typedef struct Obj0x800442d4 {
    char pad0[0x40];
    char x40[0x1A];
    short x5A;
} Obj0x800442d4;
extern int func_80034CD8(int, int);
extern int func_8002BDE4(int);
extern int func_8004A378(void *, void *);

int func_800442D4(void *arg0, int arg1)
{
    Obj0x800442d4 *o = arg0;
    Rec0x800442d4 *r;
    unsigned int i;
    unsigned int n;

    if (o == 0) {
        return arg1;
    }
    if ((unsigned int)(arg1 - 0x50) < 2) {
        if (func_80034CD8(2, 15) != 0) {
            return arg1;
        }
        i = 0;
        n = ((Tbl0x800442d4 *)WORK->x604)->count;
        r = ((Tbl0x800442d4 *)WORK->x604)->recs;
        for (; i < n; i++, r++) {
            if (r->b2 == o->x5A && func_8004A378(o->x40, r->x4)) {
                if (r->b1) {
                    func_8002BDE4((arg1 + 0xB1) & 0xFF);
                }
                if (r->b3 != 3) {
                    arg1 += (r->b3 + 1) * 2;
                }
                return arg1;
            }
        }
    } else if ((unsigned int)(arg1 - 0x30) < 2) {
        i = 0;
        n = ((Tbl0x800442d4 *)WORK->x604)->count;
        r = ((Tbl0x800442d4 *)WORK->x604)->recs;
        for (;; i++, r++) {
            if (i >= n) {
                return arg1;
            }
            if (r->b1 && r->b2 == o->x5A && func_8004A378(o->x40, r->x4)) {
                func_8002BDE4((arg1 + 0xB1) & 0xFF);
                arg1 += 2;
                break;
            }
        }
    }
    return arg1;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_8004445C);

extern Obj0x80043d94 D_800B32F0;
typedef struct Snd0x800444f0 {
    char pad0[0xE];
    signed char vol;
    char padf;
    int x10;
    int x14;
    int x18;
    int x1C;
    char pad20[0x38 - 0x20];
    unsigned char x38;
} Snd0x800444f0;
typedef struct Voice0x800444f0 {
    char pad0[4];
    short x4;
    short x6;
    short x8;
    short xA;
    char padc[0x11 - 0xC];
    unsigned char x11;
    char pad12[0x1B - 0x12];
    unsigned char x1B;
    unsigned char x1C;
    char pad1d;
    short x1E;
    char pad20[0x24 - 0x20];
} Voice0x800444f0;
typedef struct Attr0x800444f0 {
    unsigned int voice;
    unsigned int mask;
    short left;
    short right;
    short volmode_l;
    short volmode_r;
    short volx_l;
    short volx_r;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    unsigned int addr;
    unsigned int loop_addr;
    int a_mode;
    int s_mode;
    int r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
    short pad3e;
} Attr0x800444f0;
typedef struct Tbl0x800444f0 {
    signed char b[11];
} Tbl0x800444f0;
typedef struct Hdr0x800444f0 {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    unsigned char x4;
    unsigned char x5;
    char pad6[0xA - 0x6];
    unsigned short xA;
    unsigned short xC;
    unsigned short xE;
} Hdr0x800444f0;
extern Voice0x800444f0 D_800B2BC0[];
extern Tbl0x800444f0 D_80019648;
extern void func_80034C48(int, int, int);
extern void func_800708D4();
extern void func_800700F4();

void func_800444F0(void *arg0, unsigned char *arg1, int arg2, int arg3, int arg4, int arg5)
{
    Attr0x800444f0 a;
    Tbl0x800444f0 t;
    Hdr0x800444f0 *h;
    Snd0x800444f0 *o;
    Voice0x800444f0 *v;
    unsigned char ch;
    unsigned char idx;
    int bit;

    h = (Hdr0x800444f0 *)arg1;
    ch = arg4;
    o = (Snd0x800444f0 *)&D_800B32F0;
    if ((unsigned int)(arg5 - 0x10) < 0x20 && arg5 != 0x11 && arg5 != 0x21 && arg5 != 0x1A) {
        func_80034C48(2, 0x19, 1);
    }
    v = &D_800B2BC0[ch];
    bit = 1 << ch;
    v->x1C = 0;
    v->x1E = 0x7F;
    v->x1B = arg5;
    a.mask = 0x600EF;
    a.voice = bit;
    a.volmode_r = 0;
    a.volmode_l = 0;
    v->x11 = 1;
    v->xA = arg2;
    o->x18 |= bit;
    o->x10 |= bit;
    a.left = arg2 * o->vol * ((signed char *)WORK)[0x108A] / 1905;
    v->x6 = a.left;
    v->x8 = arg3;
    a.right = arg3 * o->vol * ((signed char *)WORK)[0x108A] / 1905;
    v->x4 = a.right;
    a.note = h->x4 | (h->x5 << 8);
    if (arg0 != 0 && (unsigned int)(arg5 - 0x30) < 0x21) {
        t = D_80019648;
        if (arg0 != WORK->units) {
            idx = ((unsigned int)arg0 - (unsigned int)WORK->units) / 608;
            a.note += t.b[(unsigned char)(idx % 10)] << 8;
        }
    }
    a.sample_note = h->x3 << 8;
    a.addr = h->xE << 3;
    a.adsr1 = h->xA;
    a.adsr2 = h->xC;
    func_800708D4(&a);
    if (o->x38 != 0) {
        if (h->x0 & 4) {
            o->x14 |= 1 << ch;
            func_800700F4(1, o->x14);
        } else {
            o->x1C |= 1 << ch;
            func_800700F4(0, o->x1C);
        }
    } else {
        o->x1C |= 1 << ch;
        func_800700F4(0, o->x1C);
    }
}

typedef struct Obj0x80044834 {
    char pad0[0x58];
    unsigned char x58;
} Obj0x80044834;
typedef struct Rec0x80044834 {
    signed char b0;
    signed char b1;
    signed char b2;
    signed char b3;
    signed char b4;
    signed char b5;
    signed char b6;
    signed char b7;
    signed char b8;
} Rec0x80044834;
extern unsigned char D_800AE547;
extern unsigned char *D_8008A034[];
extern signed char D_800B32D8[];
extern int func_800442D4(void *, int);
extern void func_80043E74(int, Rec0x80044834 *);
extern int func_8004445C(Rec0x80044834 *, int);
extern void func_80043F54(int, int, int *, int *);
extern void func_80044078(void *, int, int *, int *, int);
extern void func_800444F0(void *, unsigned char *, int, int, int, int);

void func_80044834(void *arg0, int arg1)
{
    Obj0x80044834 *obj;
    Rec0x80044834 rec;
    int out0;
    int out1;
    unsigned char *p;
    int r;
    int i;
    int v;
    int cond;

    i = 0;
    obj = arg0;
    if (((unsigned char *)WORK)[0x4C7] != 0 && obj != 0) {
        if (D_800AE547 == 2) {
            cond = obj->x58 & 1;
        } else {
            cond = (unsigned int)(obj->x58 - 20) < 7;
        }
        if (cond) {
            arg1 += 0x20;
        }
    }
    arg1 = func_800442D4(obj, arg1);
    func_80043E74(arg1, &rec);
    if (rec.b0 == -1) {
        return;
    }
    do {
        p = D_8008A034[rec.b0] + (((rec.b1 << 4) + rec.b2) * 16 + 0x50);
        r = func_8004445C(&rec, i);
        if (r >= 0x18) {
            return;
        }
        if (rec.b3) {
            if (D_800B32D8[r] >= rec.b4) {
                return;
            }
        } else {
            if (D_800B32D8[r] > rec.b4) {
                return;
            }
        }
        if (rec.b5) {
            func_80043F54(p[2], p[1], &out0, &out1);
        } else {
            v = 1000000;
            if (obj != 0) {
                if (obj->x58 == 3 || obj->x58 == 8 || obj->x58 == 6) {
                    v = 5000000;
                }
            }
            func_80044078(obj, p[1], &out0, &out1, v);
        }
        func_800444F0(obj, p, out0, out1, r & 0xFF, arg1);
        D_800B32D8[r] = rec.b4;
        if (rec.b8) {
            rec.b8++;
        }
        rec.b2++;
        i++;
    } while (--rec.b6 != -1);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80044A88);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80044AB4);

void func_80044B80(int arg0, int arg1, int arg2)
{
    Obj0x80043d94 *o;
    Rec0x80044834 rec;
    int out0;
    int out1;
    unsigned char *p;
    int r;
    int i;

    i = 0;
    o = &D_800B32F0;
    func_80043E74(arg0, &rec);
    if (rec.b0 == -1) {
        return;
    }
    do {
        p = D_8008A034[rec.b0] + (((rec.b1 << 4) + rec.b2) * 16 + 0x50);
        r = func_8004445C(&rec, i);
        if (r >= 0x18) {
            return;
        }
        if (rec.b3) {
            if (D_800B32D8[r] >= rec.b4) {
                return;
            }
        } else {
            if (D_800B32D8[r] > rec.b4) {
                return;
            }
        }
        func_80043F54(arg2, arg1, &out0, &out1);
        out0 = out0 * ((signed char *)WORK)[0x108A] * ((signed char *)o)[0xE] / 1905;
        out1 = out1 * ((signed char *)WORK)[0x108A] * ((signed char *)o)[0xE] / 1905;
        func_800444F0(0, p, out0, out1, r & 0xFF, arg0);
        D_800B32D8[r] = rec.b4;
        if (rec.b8) {
            rec.b8++;
        }
        rec.b2++;
        i++;
    } while (--rec.b6 != -1);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80044DC4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_8004526C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_8004538C);

extern void func_80045A20(int, int);

void func_80045460(unsigned short mask)
{
    unsigned short i;

    for (i = 0; i < 8; i++) {
        if ((mask >> i) & 1) {
            func_80045A20(i & 3, (i >> 2) + 5);
        }
    }
}

typedef struct Obj0x800454bc {
    char pad0[0xA];
    unsigned char xA;
    char padb[0x34 - 0xB];
    unsigned short x34;
    unsigned short x36;
    unsigned char x38;
} Obj0x800454bc;
typedef struct Tbl0x800454bc {
    unsigned short v[6];
} Tbl0x800454bc;
extern Tbl0x800454bc D_80019654;
extern void func_80043A7C(void);
extern unsigned short *func_800458F0(int a, int b);

void func_800454BC(void)
{
    Obj0x800454bc *o;
    unsigned short *p;
    Tbl0x800454bc t;
    unsigned short i;
    unsigned short c;

    o = (Obj0x800454bc *)&D_800B32F0;
    p = func_800458F0(WORK->xC2D, WORK->xC2C);
    c = p[1];
    o->x34 = o->x36;
    o->x36 = c;
    func_80045460(c);
    t = D_80019654;
    o->x38 = 0;
    for (i = 0; i < 6; i++) {
        if (*(unsigned short *)&WORK->xC2C == t.v[i]) {
            o->x38 = 1;
            break;
        }
    }
    if (WORK->xC2D == 7) {
        o->x38 = 1;
    }
    if (WORK->xC2D == 5) {
        o->x38 = 1;
    }
    if (*(unsigned short *)&WORK->xC2C == 0x402) {
        o->xA = 3;
        func_80043A7C();
    } else if (*(unsigned short *)&WORK->xC2C == 0x503) {
        o->xA = 3;
        func_80043A7C();
    } else {
        o->xA = 2;
        func_80043A7C();
    }
}

typedef struct Obj0x80045638 {
    unsigned char pad0[0xB];
    signed char xB;
    unsigned char padC[0x32 - 0xC];
    unsigned short x32;
} Obj0x80045638;
typedef struct Src0x80045638 {
    unsigned char pad0[0x8];
    union {
        unsigned short h;
        unsigned char b;
    } x8;
} Src0x80045638;
typedef struct Ent0x80045638 {
    unsigned char pad0[0x1A];
    signed char x1A;
    unsigned char x1B;
    unsigned char pad1C[0xEC - 0x1C];
} Ent0x80045638;
extern struct Obj0x80043d94 D_800B32F0;
extern Ent0x80045638 D_800B2F28[];
extern Src0x80045638 *func_80064998();
extern unsigned short *func_800458F0(int a, int b);
extern void func_80045B8C();
extern void func_80044A88(int, int, int, int);

void func_80045638(void)
{
    Obj0x80045638 *o = (Obj0x80045638 *)&D_800B32F0;
    Src0x80045638 *s;
    Ent0x80045638 *e;
    unsigned short i;
    unsigned short j;
    int bits;

    s = func_80064998();
    if ((*func_800458F0(s->x8.h >> 8, s->x8.b) & 0xFF00) != (o->x32 & 0xFF00)) {
        for (j = 0; j < 4; j++) {
            func_80045B8C(j & 3, 4, 0, 0x80);
        }
    } else {
        bits = func_800458F0(s->x8.h >> 8, s->x8.b)[1];
        for (i = 0; i < 8; i++) {
            if (!((bits >> i) & 1)) {
                e = &D_800B2F28[i & 3];
                if (e->x1B != 0 && e->x1A == (i >> 2) + 5) {
                    func_80045B8C(i & 3, 4, 0, 0x80);
                }
            }
        }
    }
    for (j = o->xB & 0xFFFF; j < 0x18; j++) {
        func_80044A88(j, 4, 0, 0x80);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_8004579C);

extern signed char D_800B32FB;
extern void func_80044A88(int, int, int, int);

void func_80045824(void)
{
    unsigned short i;
    int t;

    t = D_800B32FB;
    for (i = t; i < 0x18; i++) {
        func_80044A88(i, 5, 0, 0x80);
    }
}


extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


