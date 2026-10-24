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
int func_80048048(void *, void *, int, int, int, int);
void func_8004A240(void *, void *, SVECTOR *);
int func_8004A32C(void);
void func_800649A8(int, int, int);
SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
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

/* func_80041994 reads x4 (T17.2 w38), moved above it. */
typedef struct Snd0x80041dc0 {
    unsigned short id;
    char pad2[2];               /* was pad2[6] */
    unsigned int x4;            /* func_80041994: compared unsigned with Ent.cur */
} Snd0x80041dc0;
extern Snd0x80041dc0 D_800187AC[];

typedef struct Ent0x80041994 {
    unsigned short id;
    unsigned int lo;
    unsigned int cur;
} Ent0x80041994;
typedef struct Wk0x80041994 {
    char pad0[0x220];
    unsigned short x220;
} Wk0x80041994;
extern Ent0x80041994 D_80089D50[];
extern Snd0x80041dc0 D_800187AC[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);

void func_80041994(int idx)
{
    Ent0x80041994 *e;
    Wk0x80041994 *w;


    e = &D_80089D50[idx];
    if (e->cur != e->lo) {
        w = *SCRATCH(Wk0x80041994 *, 0);

        if (w->x220 != 0xFFFF) {
            if (D_800187AC[w->x220].x4 < e->cur) {
                w->x220 = 0xFFFF;
            }
        }
    }
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001EC10(e->id);
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
}

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

typedef struct Args0x80041ae4 {
    void *x0;
    void *x4;
    AreaTbl *x8;
    int xC;
    int x10;
    char *x14;
    void *x18;
    int x1C;
    int x20;
    int x24;
    void *x28;
} Args0x80041ae4;
extern void func_80041C30();

void func_80041AE4(Args0x80041ae4 *a)
{
    WORK->x5DC = a->x0;
    WORK->x5F0 = a->x4;
    WORK->areas = a->x8;
    WORK->x5E4 = a->xC;
    WORK->x5F8 = a->x10;
    WORK->x5F4 = a->x14;
    WORK->x604 = a->x18;
    WORK->x608 = 0;
    WORK->x60C = a->x28;
    *(int *)&WORK->x318[0x358 - 0x318] = a->x24;
    *(int *)&WORK->x318[0x358 - 0x318] += 0x10;
    func_80041C30(a);
}

extern int D_800AF5B8;
extern int D_800AF590;
extern int D_800AF150;
extern int D_800ABA6C;

void func_80041BB8(void)
{
    Ent0x8001c2b0 *e;

    D_800AF11C = 0;
    D_800AF590 = 0;
    D_800AF5B8 = 0;
    D_800AF150 = 0;
    D_800ABA6C = 0;
    for (e = (Ent0x8001c2b0 *)((char *)WORK + 0x4EC); e < (Ent0x8001c2b0 *)((char *)WORK + 0x5DC); e++) {
        e->flags &= ~1;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80041C30);

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

typedef struct Ent0x8004262c {
    unsigned short id;
    char pad2[6];
    int x8;
} Ent0x8004262c;
extern Ent0x8004262c D_8001945C[];
extern void func_8001CDE4(int);
extern void func_8001CED4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);
extern void func_80029F7C(void);
extern void func_80034C48(int, int, int);

void func_8004262C(int idx)
{
    Ent0x8004262c *e;

    WORK->x6F8 = 0x80;
    WORK->x6F9 = 0x80;
    WORK->x6FA = 0x80;
    WORK->x6FB = 1;
    WORK->x6FC = 0;
    e = &D_8001945C[idx];
    func_80034C48(3, 0, 0);
    while (func_8001D9D4()) {
        func_80029F7C();
        func_8001CDE4(1);
    }
    func_8001EC10(e->id);
    while (func_8001D9D4()) {
        func_80029F7C();
        func_8001CDE4(1);
    }
    WORK->x6F8 = 0xA0;
    WORK->x6F9 = 0xA0;
    WORK->x6FA = 0xA0;
    WORK->x6FB = 0;
    WORK->x6FC = 0;
    if (e->x8) {
        func_8001CED4(e->x8);
    }
}

extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);
extern void func_80029F7C(void);

void func_80042758(void)
{
    WORK->x6F8 = 0x80;
    WORK->x6F9 = 0x80;
    WORK->x6FA = 0x80;
    WORK->x6FB = 1;
    WORK->x6FC = 0;
    while (func_8001D9D4()) {
        func_80029F7C();
        func_8001CDE4(1);
    }
    func_8001ECBC(0x89, (int)D_801C1500);
    while (func_8001D9D4()) {
        func_80029F7C();
        func_8001CDE4(1);
    }
    WORK->x6F8 = 0xA0;
    WORK->x6F9 = 0xA0;
    WORK->x6FA = 0xA0;
    WORK->x6FB = 0;
    WORK->x6FC = 0;
}

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

typedef struct Obj0x800428d8 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x50 - 0x46];
    int flags;
    char pad54[0x64 - 0x54];
    int x64;
    char pad68[0x6C - 0x68];
    int x6C;
    char pad70[0xD8 - 0x70];
    short xD8;
    short xDA;
    short xDC;
    char padde[0x138 - 0xDE];
    short x138;
    char pad13a[0x13C - 0x13A];
    unsigned int x13C;
    char pad140[0x188 - 0x140];
    int x188;
    char pad18c[0x260 - 0x18C];
} Obj0x800428d8;
typedef struct Rec0x800428d8 {
    char pad0[8];
    short x8;
    short xA;
    short xC;
    short xE;
    char pad10[4];
} Rec0x800428d8;
typedef Pos0x8001d338 Vec0x800428d8;
typedef struct Work0x800428d8 {
    char pad0[0x4E0];
    Obj0x800428d8 *x4E0;
    char pad4e4[0x5F0 - 0x4E4];
    char *x5F0;
    char pad5f4[0x6F0 - 0x5F4];
    Obj0x800428d8 *x6F0;
    char pad6f4[0xC2C - 0x6F4];
    unsigned short xC2C;
} Work0x800428d8;
#define W0x800428d8 ((Work0x800428d8 *)WORK)
extern int D_800AF11C;
extern char D_800B2B20[];
char *func_800474B4(int a, int b);
extern void func_80042AE4();
extern int func_80042E84();
extern void func_80043138();

int func_800428D8(Obj0x800428d8 *obj)
{
    Vec0x800428d8 save;
    Rec0x800428d8 *base;
    Obj0x800428d8 *p;
    Obj0x800428d8 *u;

    base = (Rec0x800428d8 *)(W0x800428d8->x5F0 + 4);
    if (obj->flags & 0x100) {
        p = (Obj0x800428d8 *)func_800474B4(obj->x64, obj->x188);
        save.x = obj->x40;
        save.y = obj->x42;
        save.z = obj->x44;
        obj->x40 = p->x40;
        obj->x42 = p->x42;
        obj->x44 = p->x44;
    }
    func_80042E84(obj);
    obj->xD8 = base[obj->x138].x8;
    obj->xDA = base[obj->x138].xA;
    obj->xDC = base[obj->x138].xC;
    if (base[obj->x138].xE == -0x8000) {
        obj->xDA = obj->x42;
    }
    func_80043138(obj, D_800B2B20);
    u = W0x800428d8->x4E0 + D_800AF11C;
    u--;
    for (;;) {
        if ((u->flags & 0x20001) == 1 && u != W0x800428d8->x6F0) {
            func_80042AE4(u, obj);
            if (W0x800428d8->xC2C == 0x903) {
                u->x13C = (u->x13C << 4) / (u->x6C >> 8);
            }
        }
        if (u <= W0x800428d8->x4E0 + 1) {
            break;
        }
        u--;
    }
    if (obj->flags & 0x100) {
        obj->x40 = save.x;
        obj->x42 = save.y;
        obj->x44 = save.z;
    }
    return 0;
}

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
extern int func_80042E84();
extern void func_80043138();
extern void func_80042AE4();

int func_80042D3C(Act0x80042d3c *act, Pos0x80042d3c *pos)
{
    Obj0x80042d3c *o;
    Elem0x80042d3c *tbl;
    unsigned short y;

    tbl = ((Graph0x80042d3c *)WORK->x5F0)->e; /* was (*(Graph0x80042d3c **)WORK->pad5f0)->e */
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

typedef struct Elem0x80042e84 {
    char pad0[0x8];
    short x8[3];
    short xE;
    char pad10[0x4];
} Elem0x80042e84;
typedef struct List0x80042e84 {
    short count;
    short pad2;
    Elem0x80042e84 e[1];
} List0x80042e84;
typedef struct Obj0x80042e84 {
    char pad0[0x40];
    short x40[3];
    char pad46[0x5A - 0x46];
    short x5A;
    char pad5c[0x138 - 0x5C];
    short x138;
    short pad13a;
    unsigned int x13C;
} Obj0x80042e84;
typedef struct Pair0x80042e84 {
    unsigned int dist;
    int idx;
} Pair0x80042e84;
extern int func_800436C8();
extern int func_80043284();

int func_80042E84(Obj0x80042e84 *obj)
{
    Pair0x80042e84 arr[16];
    short *pos;
    Elem0x80042e84 *e;
    short j;
    short n;
    short k;
    int l;
    int swapped;
    unsigned int t0;
    unsigned int t1;
    int i0;
    int i1;

    pos = obj->x40;
    e = ((List0x80042e84 *)WORK->x5F0)->e;
    n = ((List0x80042e84 *)WORK->x5F0)->count;
    do {
        n--;
        arr[n].dist = func_800436C8(obj->x40, e[n].x8);
        arr[n].idx = n;
    } while (n != 0);

    swapped = 0;
    n = ((List0x80042e84 *)WORK->x5F0)->count;
    for (k = 0; k < n - 1; k++) {
        l = k + 1;
        for (j = 0; j < n - l; j++) {
            t0 = arr[j].dist;
            if (arr[j + 1].dist < t0) {
                i0 = arr[j].idx;
                swapped = 1;
                t1 = arr[j + 1].dist;
                i1 = arr[j + 1].idx;
                arr[j + 1].dist = t0;
                arr[j + 1].idx = i0;
                arr[j].dist = t1;
                arr[j].idx = i1;
            }
        }
        if (!swapped) {
            break;
        }
    }

    obj->x138 = 0;
    obj->x13C = 0x7FFFFFFF;
    for (k = 0; k < ((List0x80042e84 *)WORK->x5F0)->count; k++) {
        Elem0x80042e84 *p = &e[arr[k].idx];
        if (obj->x5A == p->xE || p->xE == -0x8000) {
            if (func_80043284(obj, pos, p->x8, 1) == 0) {
                obj->x138 = arr[k].idx;
                obj->x13C = arr[k].dist;
                break;
            }
        }
    }
    if (obj->x13C == 0x7FFFFFFF) {
        obj->x138 = arr[0].idx;
        obj->x13C = arr[0].dist;
        return 1;
    }
    return 0;
}

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

int func_800434DC(Seg *a, Seg *b)
{
    int ax0, ax1, bx0, bx1;
    int az0, az1, bz0, bz1;
    int dax, dbz, daz, dbx;
    int d, s, t, ox, oz;

    ax0 = a->x0;
    ax1 = a->x1;
    bx0 = b->x0;
    bx1 = b->x1;
    if (ax1 < ax0) {
        if (bx0 < bx1) {
            if (ax0 < bx0) return 0;
            if (bx1 < ax1) return 0;
        } else {
            if (ax0 < bx1) return 0;
            if (bx0 < ax1) return 0;
        }
    } else {
        if (bx0 < bx1) {
            if (ax1 < bx0) return 0;
            if (bx1 < ax0) return 0;
        } else {
            if (ax1 < bx1) return 0;
            if (bx0 < ax0) return 0;
        }
    }
    az0 = a->z0;
    az1 = a->z1;
    bz0 = b->z0;
    bz1 = b->z1;
    if (az1 < az0) {
        if (bz0 < bz1) {
            if (az0 < bz0) return 0;
            if (bz1 < az1) return 0;
        } else {
            if (az0 < bz1) return 0;
            if (bz0 < az1) return 0;
        }
    } else {
        if (bz0 < bz1) {
            if (az1 < bz0) return 0;
            if (bz1 < az0) return 0;
        } else {
            if (az1 < bz1) return 0;
            if (bz0 < az0) return 0;
        }
    }
    d = (ax1 - ax0) * (bz1 - bz0) - (az1 - az0) * (bx1 - bx0);
    dax = ax1 - ax0;
    dbz = bz1 - bz0;
    daz = az1 - az0;
    dbx = bx1 - bx0;
    if (d == 0) return 0;
    oz = az0 - bz0;
    ox = ax0 - bx0;
    s = oz * dbx - ox * dbz;
    t = oz * dax - ox * daz;
    return (((d > 0 && s >= 0 && s <= d) || (d < 0 && s >= d && s <= 0)) &&
            ((d > 0 && t >= 0 && t <= d) || (d < 0 && t >= d && t <= 0))) ? (unsigned int)(daz * dbx - dax * dbz) >> 31 : 0;
}

int func_800436C8(V3s_s800436C8 *a, V3s_s800436C8 *b)
{
#include "../shared/func_800436C8.inc.c"
}

typedef struct Elem0x80043714 {
    char pad0[8];
    short v[3];
    char pad0e[0x14 - 0xE];
} Elem0x80043714;
typedef struct Tbl0x80043714 {
    short count;
    short pad2;
    Elem0x80043714 e[1];
} Tbl0x80043714;
typedef struct Obj0x80043714 {
    char pad0[0x138];
    short idx;
    unsigned char flag;
} Obj0x80043714;
int func_8004A32C(void);

void func_80043714(Obj0x80043714 *obj, short *out)
{
    Elem0x80043714 *e = ((Tbl0x80043714 *)WORK->x5F0)->e;
    int r = func_8004A32C();

    obj->idx = r % ((Tbl0x80043714 *)WORK->x5F0)->count;
    obj->flag = 1;
    out[0] = e[obj->idx].v[0];
    out[1] = e[obj->idx].v[1];
    out[2] = e[obj->idx].v[2];
}

typedef struct Attr0x800437f8 {
    int mask;
    int x4;
    short x8;
    short xA;
    char padC[0x18 - 0xC];
} Attr0x800437f8;
extern struct Obj0x80043d94 D_800B32F0;
void func_80018020(void *, int);
extern void func_8006E514();
extern void func_80070654(int a);
extern void func_8006F6F4(Attr0x800437f8 *a);
extern void func_8006F4F4(int a);
extern int func_80071484(int arg0);
extern void func_800700A4(int a);
extern void func_80070124(int a);
extern void func_8006F204(int a, int b);
extern void func_80043BF8();

void func_800437F8(void)
{
    Attr0x800437f8 attr;

    func_80018020(&D_800B32F0, 0x1F);
    func_8006E514();
    attr.mask = 7;
    attr.x4 = 0;
    attr.x8 = 0;
    attr.xA = 0;
    func_80070654(0);
    func_8006F6F4(&attr);
    func_8006F4F4(0);
    func_80071484(0x1E);
    func_800700A4(1);
    func_80070124(7);
    func_8006F204(0, 0xFFFFFF);
    func_80043BF8();
}

typedef struct Obj0x80043884 {
    char pad0[8];
    short x8;
    unsigned char xA;
    unsigned char xB;
    char padC[0xE - 0xC];
    unsigned char xE;
    char padF[0x30 - 0xF];
    unsigned short x30;
    unsigned short x32;
    unsigned short x34;
    unsigned short x36;
} Obj0x80043884;
extern struct Obj0x80043d94 D_800B32F0;
extern Voice0x800444f0 D_800B2BC0[];
extern struct Ent0x80045638 D_800B2F28[];
extern void func_80043C8C();

void func_80043884(void)
{
    Obj0x80043884 *o = (Obj0x80043884 *)&D_800B32F0;
    Voice0x800444f0 *v;
    unsigned char i;

    o->xE = 0x7F;
    o->xB = 0x10;
    o->xA = 2;
    o->x8 = 0x3FFF;
    func_80043C8C(WORK->x1088);
    func_80018020(D_800B2F28, 0xEC);
    func_80018020(D_800B2BC0, 0xD8);
    func_80018020(D_801EED00, 0x100);
    for (i = 0; i < 24; i++) {
        v = &D_800B2BC0[i];
        v->xF = 0xFF;
        v->x16 = 0xFF;
    }
    o->x32 = 0xFFFF;
    o->x30 = 0xFFFF;
    o->x36 = 0xFFFF;
    o->x34 = 0xFFFF;
}

typedef struct SpuVol0x80043958 {
    short left;
    short right;
} SpuVol0x80043958;
typedef struct SpuExt0x80043958 {
    SpuVol0x80043958 volume;
    int reverb;
    int mix;
} SpuExt0x80043958;
typedef struct SpuAttr0x80043958 {
    unsigned int mask;
    SpuVol0x80043958 mvol;
    SpuVol0x80043958 mvolmode;
    SpuVol0x80043958 mvolx;
    SpuExt0x80043958 cd;
    SpuExt0x80043958 ext;
} SpuAttr0x80043958;
extern int D_800B2F20;
extern void func_80043884();
extern void func_800710D4(SpuAttr0x80043958 *);
extern void func_800704E4(int);
extern void func_80043C8C();
extern void func_80043A7C(void);
extern int func_800814D8(int);
extern int func_80081324(void);
extern int func_80081274(int, int, int, void (*)());
extern void func_80081334(void);
extern int func_800813D4(int, int, int);
extern int func_800812B4(int);
extern int func_800814A8(int);
extern void func_80045D68();

void func_80043958(void)
{
    SpuAttr0x80043958 attr;

    func_80043884();
    attr.mask = 0x2CF;
    attr.mvol.left = attr.mvol.right = 0x3FFF;
    attr.mvolmode.left = attr.mvolmode.right = 0;
    attr.cd.volume.left = attr.cd.volume.right = 0x7FFF;
    attr.cd.mix = 1;
    func_800710D4(&attr);
    func_800704E4(0);
    func_80043C8C(1);
    func_80043A7C();
    func_800814D8(0xF2000002);
    func_80081324();
    D_800B2F20 = func_80081274(0xF2000002, 2, 0x1000, func_80045D68);
    func_80081334();
    func_800813D4(0xF2000002, 0x251C, 0x1000);
    func_800812B4(D_800B2F20);
    func_800814A8(0xF2000002);
}

#define M2C_UNK s32
void func_80043BF8(); /* was M2C_UNK (T10 w39 draft func_800437F8) */
void func_8006F4F4(int); /* was M2C_UNK (M2C_UNK) (T10 w39 draft func_800437F8) */
void func_80070124(int); /* was M2C_UNK (M2C_UNK) (T10 w39 draft func_800437F8) */
M2C_UNK func_80081284(s32);
extern int func_80081324(void);
extern void func_80081334(void);
extern int D_800B2F20;

void func_80043A2C(void) {
    ((M2C_UNK (*)())func_80081324)();
    func_80081284(D_800B2F20);
    ((M2C_UNK (*)())func_80081334)();
    func_80043BF8();
    func_8006F4F4(0);
    func_80070124(7);
}

typedef struct Obj0x80043a7c {
    char pad0[8];
    short x8;
    signed char xA;
    char padB;
    unsigned char xC;
} Obj0x80043a7c;
extern struct Obj0x80043d94 D_800B32F0;
extern void func_8006F6F4(Attr0x800437f8 *a);
extern void func_8006F4F4(int a);
extern void func_800700A4(int a);
extern void func_80070124(int a);
extern void func_80071454(int, int);

void func_80043A7C(void)
{
    Obj0x80043a7c *o = (Obj0x80043a7c *)&D_800B32F0;
    Attr0x800437f8 attr;

    func_800700A4(1);
    func_80070124(o->xA);
    attr.mask = 7;
    attr.x4 = o->xA;
    attr.x8 = attr.xA = o->x8;
    func_8006F6F4(&attr);
    func_80071454(o->x8, o->x8);
    if (o->xA == 0) {
        o->xC &= ~2;
        func_8006F4F4(0);
    } else {
        o->xC |= 2;
        func_8006F4F4(1);
    }
}

extern void func_80045C18(int arg0);

void func_80043B24(void)
{
    unsigned char i;

    for (i = 0; i < 4; i++) {
        func_80045C18(i);
    }
}

extern signed char D_800B32D8[];
extern void func_800700F4();
typedef struct Obj0x80043b60 {
    char pad0[0xB];
    unsigned char xB;
    char padc[0x18 - 0xC];
    int x18;
    int x1C;
} Obj0x80043b60;

void func_80043B60(void)
{
    Obj0x80043b60 *o = (Obj0x80043b60 *)&D_800B32F0;
    unsigned char i;

    for (i = o->xB; i < 24; i++) {
        o->x18 |= 1 << i;
        o->x1C |= 1 << i;
        D_800B2BC0[i].x11 = 0;
        D_800B32D8[i] = -1;
    }
    func_800700F4(0, o->x1C);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800418DC", func_80043BF8);

typedef struct Obj0x80043c8c {
    unsigned char pad0[0xC];
    unsigned char flagsC;
} Obj0x80043c8c;
extern struct Obj0x80043d94 D_800B32F0;

void func_80043C8C(int on)
{
    if (on != 0) {
        ((Obj0x80043c8c *)&D_800B32F0)->flagsC |= 1;
    } else {
        ((Obj0x80043c8c *)&D_800B32F0)->flagsC &= ~1;
    }
}

typedef struct Obj0x80043d94 {
    unsigned char pad0[0xF];
    signed char count;
} Obj0x80043d94;
extern Obj0x80043d94 D_800B32F0;

void func_80043CC4(int mode)
{
    Obj0x80043d94 *o = &D_800B32F0;
    SpuAttr0x80043958 attr;
    unsigned short v;

    if (mode == 1) {
        v = WORK->x108A * 8;
    } else if (mode == 2) {
        v = WORK->x108A * 127 / 15;
    } else {
        v = WORK->x108A * 6;
    }
    o->count = v;
    attr.mask = 0xC0;
    attr.cd.volume.right = attr.cd.volume.left = 0x78F0;
    func_800710D4(&attr);
}

extern void func_80043E1C(unsigned int); /* was (int) (T10 w39 draft) */

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

typedef Sub0x80066a8c Vol0x80043e1c;
extern unsigned char D_800B32FC;
extern void func_80076D14(Vol0x80043e1c *);

void func_80043E1C(unsigned int a0)
{
    Vol0x80043e1c v;

    if (D_800B32FC & 1) {
        v.x0 = a0;
        v.x1 = 0;
        v.x2 = a0;
        v.x3 = 0;
    } else {
        unsigned int h = a0 >> 1;
        v.x0 = h;
        v.x1 = h;
        v.x2 = h;
        v.x3 = h;
    }
    func_80076D14(&v);
}

typedef struct Src0x80043e74 {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
} Src0x80043e74;
typedef struct Out0x80043e74 {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
    unsigned char x4;
    unsigned char x5;
    unsigned char x6;
    unsigned char x7;
    unsigned char x8;
    unsigned char x9;
    unsigned char xA;
    unsigned char xB;
    unsigned char xC;
} Out0x80043e74;

void func_80043E74(int idx, Out0x80043e74 *out)
{
    Src0x80043e74 *s = (Src0x80043e74 *)D_801EED00;

    s += idx;
    out->x0 = s->b1;
    out->x1 = s->b2 & 0x7F;
    out->x2 = s->b3 >> 4;
    out->x4 = s->b3 & 7;
    out->x3 = (s->b3 >> 3) & 1;
    out->x5 = (s->b0 >> 1) & 1;
    out->x6 = (s->b0 >> 2) & 3;
    out->x7 = s->b0 & 1;
    out->x8 = s->b4 & 0x1F;
    out->x9 = (s->b4 >> 5) & 1;
    out->xA = s->b5;
    out->xB = s->b6;
    out->xC = s->b7;
}

extern unsigned char D_800B32FC;

void func_80043F54(int a0, int a1, int *p2, int *p3)
{
    if ((unsigned int)a0 < 0x40) {
        *p2 = 0x7E;
        *p3 = a0 + 0x3F;
    } else {
        *p2 = 0xBE - a0;
        *p3 = 0x7E;
    }
    if (!(D_800B32FC & 1)) {
        if ((unsigned int)*p2 < (unsigned int)*p3) {
            *p2 = *p3;
        } else {
            *p3 = *p2;
        }
    }
    *p2 = (unsigned int)(*p2 * a1) / 126;
    *p2 = (unsigned int)(*p2 * *p2 * 16383) / 16129;
    *p3 = (unsigned int)(*p3 * a1) / 126;
    *p3 = (unsigned int)(*p3 * *p3 * 16383) / 16129;
}

extern unsigned int func_8004A194();
extern int func_8004A2C4(void *p, int arg);
extern void func_80043F54(int, int, int *, int *);

void func_80044078(void *obj, int vol, int *outL, int *outR, int dist)
{
    unsigned int d;
    unsigned int pan;
    unsigned int t;

    if (obj != 0) {
        d = func_8004A194(&WORK->x318[0x350 - 0x318], (char *)obj + 0x40);
        if ((unsigned int)dist < d) {
            *outR = 0;
            *outL = 0;
            return;
        }
        dist = (unsigned int)(vol * (dist - d)) / (unsigned int)dist;
        pan = func_8004A2C4(&WORK->x318[0x350 - 0x318], (int)((char *)obj + 0x40));
        pan -= *(short *)&WORK->x318[0x33A - 0x318];
        pan &= 0xFFF;
        t = pan - 0x400;
        if (t >= 0x800) {
            pan += 0x400;
            pan &= 0xFFF;
            pan >>= 4;
        } else {
            pan = t & 0xFFF;
            pan >>= 4;
            pan = 0x7F - pan;
        }
    } else {
        pan = 0x3F;
        dist = vol;
    }
    func_80043F54(pan, dist, outL, outR);
}

extern signed char D_800B32D8[];
extern signed char D_800B32FB;

int func_800441A0(int arg0, int arg1)
{
    int best;
    int min;
    int i;

    best = 0;
    min = 0x7FFF;
    for (i = D_800B32FB; i < 0x18; i++) {
        if (D_800B32D8[i] == -1) {
            best = i;
            break;
        }
        if (D_800B32D8[i] < min) {
            min = D_800B32D8[i];
            best = i;
        }
    }
    return (signed char)best;
}

extern signed char D_800B32D8[];

int func_80044220(unsigned char arg0, unsigned char mask)
{
    int best;
    int min;
    int i;

    best = arg0;
    min = 0x7FFF;
    for (i = 0; i < 8; i++) {
        if ((mask >> i) & 1) {
            if (D_800B32D8[i + arg0] == -1) {
                best = i + arg0;
                break;
            }
            if (D_800B32D8[i + arg0] < min) {
                min = D_800B32D8[i + arg0];
                best = i + arg0;
            }
        }
    }
    return (signed char)best;
}

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

typedef struct Rec0x8004445c {
    signed char pad0[8];
    signed char b8;
    signed char b9;
    unsigned char b10;
} Rec0x8004445c;
extern signed char D_800B32FB;
extern int func_800441A0(int, int);
extern int func_80044220(unsigned char, unsigned char);

int func_8004445C(Rec0x8004445c *r, int arg1)
{
    unsigned short lim = D_800B32FB;

    if (r->b8 < lim) {
        arg1 = func_800441A0(r->b8, arg1);
    } else if (r->b9 != 0) {
        arg1 = func_80044220(r->b8, r->b10);
    } else {
        int t = r->b8 + arg1;
        arg1 = t;
    }
    if (arg1 < lim) {
        arg1 = 0x18;
    }
    return arg1;
}

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
/* Voice0x800444f0: dc2.h (T8 w36) */
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
extern void func_80043E74(); /* defined above with its own out shape (T8 w36) */
extern int func_8004445C(); /* defined above with its own rec shape (T8 w37) */
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

extern Voice0x800444f0 D_800B2BC0[];

void func_80044A88(unsigned short ch, unsigned char b, short h, unsigned char a)
{
    Voice0x800444f0 *v = &D_800B2BC0[ch];

    v->x1C = a;
    v->x1D = b;
    v->x20 = h;
}

typedef struct Sys0x80044ab4 {
    unsigned char pad0[0xB];
    signed char xB;
    unsigned char padC[0x1C - 0xC];
    int x1C;
} Sys0x80044ab4;
extern void func_80044A88(unsigned short, unsigned char, short, unsigned char);
extern void func_800700F4();

void func_80044AB4(int arg0)
{
    Sys0x80044ab4 *o = (Sys0x80044ab4 *)&D_800B32F0;
    Voice0x800444f0 *v = &D_800B2BC0[o->xB];
    unsigned short i;

    for (i = o->xB; i < 24; i++, v++) {
        if ((signed char)v->x1B == arg0) {
            func_80044A88(i, 4, 0, 0x80);
            o->x1C |= 1 << i;
            func_800700F4(0, o->x1C);
        }
    }
}

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

typedef struct Ent0x8004526c {
    unsigned char pad0[0x22];
    short x22;
    short x24;
    unsigned char pad26[0x28 - 0x26];
    unsigned char x28;
    unsigned char pad29[0xEC - 0x29];
} Ent0x8004526c;
typedef struct Obj0x8004526c {
    unsigned char pad0[0xB];
    signed char xB;
} Obj0x8004526c;
extern struct Ent0x80045638 D_800B2F28[];
extern void func_80070684(int, int, unsigned char *);
extern void func_80045B8C();
extern void func_80044A88(unsigned short, unsigned char, short, unsigned char);

void func_8004526C(void)
{
    unsigned char buf[0x18];
    Ent0x8004526c *e;
    Obj0x8004526c *o;
    unsigned int i;

    e = (Ent0x8004526c *)D_800B2F28;
    o = (Obj0x8004526c *)&D_800B32F0;
    func_80070684(0, 0x17, buf);
    for (i = 0; i < 4; i++, e++) {
        e->x24 = -1;
        if (e->x22 >= 0x5B && !(e->x28 & 0x80)) {
            e->x24 = e->x22;
            func_80045B8C(i & 0xFFFF, 1, 0x5A, 1);
        }
    }
    for (i = o->xB; i < 0x18; i++) {
        if (buf[i] == 1 && !((signed char)D_800B2BC0[i].x1C & 0x80)) {
            func_80044A88(i & 0xFFFF, 4, 0x32, 1);
        }
    }
}

void func_8004538C(void)
{
    Ent0x8004526c *e;
    Obj0x8004526c *o;
    Voice0x800444f0 *v;
    unsigned int i;

    o = (Obj0x8004526c *)&D_800B32F0;
    e = (Ent0x8004526c *)D_800B2F28;
    for (i = 0; i < 4; i++, e++) {
        if (e->x24 != -1) {
            func_80045B8C(i & 0xFFFF, 2, (unsigned short)e->x24, 2);
        }
    }
    i = o->xB;
    v = &D_800B2BC0[i];
    for (; i < 0x18; i++, v++) {
        if (!((signed char)v->x1C & 0x80)) {
            func_80044A88(i & 0xFFFF, 4, 0x7F, 2);
        }
    }
}

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
extern void func_80044A88(unsigned short, unsigned char, short, unsigned char);

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

extern struct Obj0x80043d94 D_800B32F0;
typedef struct Obj0x8004579c {
    char pad0[0xB];
    signed char xB;
} Obj0x8004579c;
extern void func_80044A88(unsigned short, unsigned char, short, unsigned char);
extern void func_80045B8C();

void func_8004579C(void)
{
    Obj0x8004579c *o = (Obj0x8004579c *)&D_800B32F0;
    unsigned short i;
    int t;

    for (i = 0; i < 4; i++) {
        func_80045B8C(i, 4, 0, 0x80);
    }
    t = o->xB;
    i = t;
    while (1) {
        if (i >= 24) {
            break;
        }
        func_80044A88(i, 4, 0, 0x80);
        i++;
    }
}

extern signed char D_800B32FB;
extern void func_80044A88(unsigned short, unsigned char, short, unsigned char);

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


