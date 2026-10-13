#include "common.h"
#include "dc2.h"
#include "../shared/func_800436C8.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;




INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800458F0);

typedef struct Hdr0x80045a20 {
    unsigned short pad0;
    unsigned short div;
    unsigned int num;
} Hdr0x80045a20;
typedef struct Chan0x80045a20 {
    unsigned char *p0;
    unsigned char *p4;
    int pad8;
    int fC;
    int f10;
    int f14;
    char pad18;
    unsigned char f19;
    unsigned char f1A;
    unsigned char f1B;
    char pad1C[4];
    short f20;
    short f22;
    char pad24[4];
    unsigned char f28;
    unsigned char f29;
    unsigned char f2A;
    unsigned char f2B;
    char f2C[0xEC - 0x2C];
} Chan0x80045a20;
extern Chan0x80045a20 D_800B2F28[];
extern Hdr0x80045a20 *D_800B2BA0[];
extern void func_80018020(void *, int);
extern int func_80034CD8(int, int);
extern void func_80045B8C();
extern int func_80081324(void);
extern void func_80081334(void);

void func_80045A20(int idx, int arg1)
{
    Chan0x80045a20 *ch = &D_800B2F28[idx];
    Hdr0x80045a20 *h = D_800B2BA0[idx];

    if (func_80034CD8(0, 10) != 0) {
        return;
    }
    if (D_800B2BA0[idx] == 0) {
        return;
    }
    if (ch->f1B == 1) {
        if (ch->f28 & 0x80) {
            func_80045B8C(idx & 0xFFFF, 5, 0x7F, 2);
        }
        return;
    }
    func_80081324();
    ch->p4 = (unsigned char *)h + 8;
    ch->p0 = (unsigned char *)h + 8;
    ch->f1A = arg1;
    ch->f19 = idx;
    ch->f14 = h->div;
    ch->fC = 0;
    ch->f10 = h->num * 1000 / h->div;
    ch->f28 = 1;
    ch->f29 = 4;
    ch->f20 = 0x7F;
    ch->f22 = 0x7F;
    ch->f2A = 0x7F;
    ch->f2B = 0x7F;
    ch->f1B = 1;
    func_80018020(ch->f2C, 3);
    func_80081334();
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80045B8C);

typedef struct Voice0x80045c18 {
    int x0;
    char pad4[0xF - 0x4];
    unsigned char xF;
    char pad10;
    unsigned char x11;
    char pad12[0x16 - 0x12];
    unsigned char x16;
    char pad17[0x24 - 0x17];
} Voice0x80045c18;
typedef struct Snd0x80045c18 {
    char pad0[0xB];
    signed char xB;
    char padC[0x20 - 0xC];
    int x20;
    int x24;
    int x28;
    int x2C;
} Snd0x80045c18;
typedef struct Attr0x80045c18 {
    int voice;
    int mask;
    short left;
    short right;
    char padC[0x40 - 0xC];
} Attr0x80045c18;
extern Voice0x80045c18 D_800B2BC0[];
extern Snd0x80045c18 D_800B32F0;
extern void func_800700F4();
extern void func_800702C4();
extern void func_800708D4();

void func_80045C18(short arg0)
{
    Attr0x80045c18 a;
    Chan0x80045a20 *ch;
    Snd0x80045c18 *o;
    Voice0x80045c18 *v;
    unsigned int i;
    int bits;
    int idx;

    idx = arg0;
    ch = &D_800B2F28[idx];
    o = &D_800B32F0;
    func_80081324();
    if (ch->f1B == 1) {
        v = D_800B2BC0;
        i = 0;
        bits = 0;
        ch->f1B = 0;
        ch->f29 = 0;
        ch->f28 = 0;
        ch->f20 = 0;
        ch->f22 = 0;
        for (; i < o->xB; i++, v++) {
            if (v->x16 == idx) {
                bits |= 1 << i;
                v->x16 = 0xFF;
                v->xF = 0xFF;
                v->x11 = 0;
                v->x0 = 0;
                o->x20 &= ~bits;
                o->x28 |= bits;
                o->x24 &= ~bits;
                o->x2C |= bits;
            }
        }
        a.mask = 3;
        a.voice = bits;
        a.left = 0;
        a.right = 0;
        func_800708D4(&a);
        func_800700F4(0, bits);
        func_800702C4(0, bits);
    }
    func_80081334();
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80045D68);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046338);

typedef struct A0x80046470 {
    char pad0[0x28];
    unsigned int m28;
    unsigned int m2C;
} A0x80046470;
typedef struct B0x80046470 {
    char pad0[0x19];
    signed char m19;
} B0x80046470;
typedef struct C0x80046470 {
    unsigned char pad0[2];
    unsigned char m2;
    unsigned char m3;
    unsigned char m4;
    unsigned char m5;
    unsigned char m6;
} C0x80046470;
extern int func_800472F4(int a0, int a1, int a2, int a3);

void func_80046470(A0x80046470 *a, B0x80046470 *b, C0x80046470 *c, unsigned int mask)
{
    c->m3 = 0;
    while (mask != 0) {
        if (mask & 1) {
            c->m6 = func_800472F4(b->m19, c->m2, c->m3, c->m4);
            if (c->m6 != 0xFF) {
                a->m28 |= 1 << c->m6;
                a->m2C |= 1 << c->m6;
            }
        }
        mask >>= 1;
        c->m3++;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004653C);

int func_800469D0(void)
{
#include "../shared/func_800469D0.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800469D8);

typedef struct Prg0x80046bbc {
    char pad0[0xC];
    unsigned char xC;
    unsigned char xD;
    char padE[0x11 - 0xE];
    struct {
        unsigned char x0;
        unsigned char x1;
        char pad2[2];
    } e[1];
} Prg0x80046bbc;
typedef struct Seq0x80046bbc {
    unsigned char *ptr;
    char pad4[0x1A - 0x4];
    signed char x1A;
} Seq0x80046bbc;
typedef struct Ch0x80046bbc {
    char pad0[2];
    unsigned char x2;
    char pad3[0xA - 0x3];
    unsigned char xA;
    unsigned char xB;
} Ch0x80046bbc;
extern Prg0x80046bbc *D_8008A034[];

int func_80046BBC(int a0, Seq0x80046bbc *s, Ch0x80046bbc *c)
{
    Prg0x80046bbc *p;
    int t;

    p = D_8008A034[s->x1A];
    c->x2 = *s->ptr;
    s->ptr++;
    c->xA = (p->xC * p->e[c->x2].x0) / 127;
    t = p->xD + 0xC0;
    c->xB = p->e[c->x2].x1 + t;
    return 0;
}

typedef struct Voice0x80046c58 {
    char pad0[0xC];
    unsigned short xC;
    char padE;
    unsigned char xF;
    char pad10[0x16 - 0x10];
    unsigned char x16;
    char pad17;
    unsigned char x18;
    char pad19[0x24 - 0x19];
} Voice0x80046c58;
typedef struct Ctx0x80046c58 {
    char pad0[0xB];
    signed char xB;
    char padC;
    unsigned char xD;
} Ctx0x80046c58;
typedef struct Seq0x80046c58 {
    unsigned char *ptr;
    char pad4[0x18 - 0x4];
    unsigned char x18;
    signed char x19;
} Seq0x80046c58;

int func_80046C58(Ctx0x80046c58 *o, Seq0x80046c58 *s, unsigned short *out)
{
    Voice0x80046c58 *v;
    unsigned int i;

    *out = *s->ptr;
    *out |= *++s->ptr << 7;
    s->ptr++;
    v = (Voice0x80046c58 *)D_800B2BC0;
    for (i = 0; i < o->xB; i++, v++) {
        if (v->xF == s->x18 && s->x19 == v->x16) {
            v->xC = *out;
            v->x18 = 1;
            o->xD = 1;
        }
    }
    return 0;
}

extern Chan0x80045a20 D_800B2F28[];
extern Voice0x80045c18 D_800B2BC0[];
extern Snd0x80045c18 D_800B32F0;
extern void func_800700F4();
extern void func_800702C4();
extern void func_800708D4();

void func_80046D10(short arg0)
{
    Attr0x80045c18 a;
    Chan0x80045a20 *ch;
    Snd0x80045c18 *o;
    Voice0x80045c18 *v;
    unsigned int i;
    int bits;
    int idx;

    idx = arg0;
    ch = &D_800B2F28[idx];
    o = &D_800B32F0;
    if (ch->f1B == 1) {
        v = D_800B2BC0;
        i = 0;
        bits = 0;
        ch->f1B = 0;
        for (; i < o->xB; i++, v++) {
            if (v->x16 == idx) {
                bits |= 1 << i;
                v->x16 = 0xFF;
                v->xF = 0xFF;
                v->x11 = 0;
                v->x0 = 0;
                o->x20 &= ~bits;
                o->x28 |= bits;
                o->x24 &= ~bits;
                o->x2C |= bits;
            }
        }
        a.mask = 3;
        a.voice = bits;
        a.left = 0;
        a.right = 0;
        func_800708D4(&a);
        func_800700F4(0, bits);
        func_800702C4(0, bits);
    }
}

typedef struct Seq0x80046e2c {
    unsigned char *ptr;             
    int pad4;                       
    unsigned char *loop;            
    int padC;                       
    int tempo;                      
    int div;                        
    unsigned char pad18;            
    signed char ch;                 
    unsigned char pad1A;            
    unsigned char flag1B;           
    unsigned char pad1C;            
    unsigned char mode;             
    unsigned char count;            
} Seq0x80046e2c;
extern void func_80046D10();

int func_80046E2C(int a0, Seq0x80046e2c *s, int a2, int *out)
{
    unsigned char *p;
    int v;

    if (s->ptr[0] == 0x2F && s->ptr[1] == 0) {
        if (s->mode == 2) {
            if (s->count == 0x7F || (s->count = s->count - 1) != 0) {
                s->ptr = s->loop;
            }
            s->mode = 0;
        } else {
            func_80046D10(s->ch);
        }
        if (s->flag1B == 0) {
            *out = 0x80;
        } else {
            *out = 0;
        }
    }
    p = s->ptr;
    if (*p == 0x51) {
        s->ptr = p + 1;
        v = p[1] << 16;
        s->ptr = p + 2;
        v |= p[2] << 8;
        s->ptr = p + 3;
        v |= p[3];
        s->ptr = p + 4;
        s->tempo = (v * 1000) / s->div;
    }
    return 0;
}

typedef struct Ent0x80046f6c {
    unsigned char pad0[5];
    unsigned char lo;
    unsigned char hi;
    unsigned char pad7[9];
} Ent0x80046f6c;

int func_80046F6C(Ent0x80046f6c *p, unsigned int v, unsigned int n)
{
    unsigned int i;
    int mask = 0;

    for (i = 0; i < n; i++, p++) {
        if (v >= p->lo && p->hi >= v) {
            mask |= 1 << i;
        }
    }
    return mask;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046FC0);

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

void func_80047168(void *arg0, s16 arg1, u32 arg2) {
    M2C_FIELD(arg0, s16 *, 6) = arg1;
    M2C_FIELD(arg0, s16 *, 4) = arg1;
    if (arg2 < 0x40U) {
        M2C_FIELD(arg0, s16 *, 4) = (s16) ((s32) (arg1 * (s16) arg2) >> 6);
    }
    if (arg2 >= 0x41U) {
        M2C_FIELD(arg0, s16 *, 6) = (s16) ((s32) (arg1 * (s16) (0x7F - arg2)) >> 6);
    }
}

extern Voice0x80045c18 D_800B2BC0[];
extern Snd0x80045c18 D_800B32F0;
extern void func_80070684();

int func_800471CC(void)
{
    unsigned char buf[24];
    Voice0x80045c18 *v;
    Snd0x80045c18 *s;
    signed char i;
    unsigned int min;
    signed char best;

    v = D_800B2BC0;
    s = &D_800B32F0;
    func_80070684(0, s->xB - 1, buf);
    for (i = 0; i < s->xB; v++, i++) {
        if (buf[i] == 0 && v->x11 == 0) {
            return i;
        }
    }
    v = D_800B2BC0;
    min = 1;
    best = -1;
    for (i = 0; i < s->xB; v++, i++) {
        if (buf[i] == 3) {
            return i;
        }
        if ((unsigned int)v->x0 >= min) {
            min = v->x0;
            best = i;
        }
    }
    return best;
}

typedef struct Voice0x800472f4 {
    int x0;
    char pad4[0xF - 0x4];
    unsigned char xF;
    char pad10;
    unsigned char x11;
    unsigned char x12;
    unsigned char x13;
    unsigned char x14;
    char pad15;
    unsigned char x16;
    char pad17[0x24 - 0x17];
} Voice0x800472f4;
extern signed char D_800B32FB;

int func_800472F4(int a0, int a1, int a2, int a3)
{
    Voice0x800472f4 *v = (Voice0x800472f4 *)D_800B2BC0;
    signed char i;

    for (i = 0; i < D_800B32FB; v++, i++) {
        if (v->x12 == a1 && v->x13 == a2 && v->x14 == a3 && v->x11 != 0 && a0 == v->x16) {
            v->x11 = 0;
            v->x0 = 0;
            v->xF = 0xFF;
            v->x16 = 0xFF;
            return i;
        }
    }
    return 0xFF;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800473A4);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047450);

typedef struct Node0x800474b4 {
    unsigned char pad0[0x54];
    struct Node0x800474b4 *next;
} Node0x800474b4;

void *func_800474B4(int a, int b)
{
    Node0x800474b4 *p = (Node0x800474b4 *)a;
    while (b--) {
        p = p->next;
    }
    return p;
}

typedef struct Ent0x800474dc {
    unsigned char x0;
    char pad1[0x34 - 0x1];
} Ent0x800474dc;
extern int D_800AF5B8;
extern void func_80018020(void *, int);

Ent0x800474dc *func_800474DC(void)
{
    Ent0x800474dc *p;
    int n;
    int i;

    n = D_800AF5B8;
    p = &((Ent0x800474dc *)WORK->x4E8)[n - 1];
    i = n - WORK->x614 - 1;
    while (i-- != 0) {
        if (!(p->x0 & 1)) {
            func_80018020(p, 0xD);
            p->x0 = 1;
            return p;
        }
        p--;
    }
    return 0;
}

void func_80047584(unsigned char *p)
{
#include "../shared/func_80047584.inc.c"
}

typedef struct Sub0x8004758c {
    char pad0[1];
    unsigned char x1;
    char pad2[0x18 - 0x2];
    unsigned char x18;
} Sub0x8004758c;
typedef struct Ent0x8004758c {
    char pad0[0x50];
    int x50;
    char pad54[0x64 - 0x54];
    unsigned char x64;
    unsigned char x65;
    unsigned char x66;
    unsigned char x67;
    char pad68[0x86 - 0x68];
    unsigned char x86;
    unsigned char x87;
    char pad88[0x8C - 0x88];
    Sub0x8004758c *x8C;
    char pad90[0xAC - 0x90];
} Ent0x8004758c;
extern unsigned int D_800AF590;
extern void func_80018020(void *, int);

Ent0x8004758c *func_8004758C(void)
{
    Ent0x8004758c *p;
    Ent0x8004758c *q;
    Sub0x8004758c *s;
    unsigned int i;
    unsigned char next;
    unsigned char n;
    Work *w;
    int idx;

    w = WORK;
    idx = w->x3EE;
    if (idx == 0xFF) {
        return 0;
    }
    p = (Ent0x8004758c *)w->x4E4 + idx;
    for (i = 0; i < D_800AF590;) {
        i++;
        if (p->x86 == 0xFF) {
            n = p->x87;
            s = p->x8C;
            q = (Ent0x8004758c *)WORK->x4E4 + n;
            if (p == q) {
                WORK->x3EE = 0xFF;
                next = n;
            } else {
                next = q->x86;
                q->x86 = 0xFF;
            }
            if (s != 0 && s->x1 == 1) {
                s->x18--;
            }
            func_80018020(p, 0x2B);
            p->x50 = 1;
            p->x64 = p->x65 = p->x66 = 0x80;
            p->x67 = 0x2C;
            p->x87 = next;
            p->x86 = next;
            return p;
        }
        p = (Ent0x8004758c *)WORK->x4E4 + p->x86;
    }
    return 0;
}

extern Ent0x8004758c *func_8004758C(void);

Ent0x8004758c *func_80047714(void)
{
    Ent0x8004758c *p;
    int n;
    int i;

    n = D_800AF590;
    i = n - WORK->x610;
    p = &((Ent0x8004758c *)WORK->x4E4)[n - 1];
    while (i-- != 0) {
        if (!(p->x50 & 1)) {
            func_80018020(p, 0x2B);
            p->x50 = 1;
            p->x64 = p->x65 = p->x66 = 0x80;
            p->x67 = 0x2C;
            p->x86 = p->x87 = (unsigned char)WORK->x610 + i;
            return p;
        }
        p--;
    }
    return func_8004758C();
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047814);

typedef struct Ent0x800478b8 {
    char pad0[0x50];
    int x50;
    char pad54[0x70 - 0x54];
} Ent0x800478b8;
extern int D_800AF150;
extern void func_80018020(void *, int);

Ent0x800478b8 *func_800478B8(void)
{
    Ent0x800478b8 *p;
    int n;
    int i;

    n = D_800AF150;
    p = &((Ent0x800478b8 *)WORK->x4DC)[n - 1];
    i = n - WORK->x618 - 12;
    while (i-- != 0) {
        if (!(p->x50 & 1)) {
            func_80018020(p, 0x1C);
            p->x50 = 1;
            return p;
        }
        p--;
    }
    return 0;
}

void func_80047958(int *a0)
{
#include "../shared/func_80047958.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047960);

extern void func_80047AD4();

Unit *func_800479FC(void)
{
    unsigned int i;
    Unit *u;

    for (i = D_800AF11C - 1; WORK->x61C < i; i--) {
        u = &WORK->units[i];
        if (!(u->flags & 1)) {
            func_80047AD4(i);
            return u;
        }
    }
    return 0;
}

typedef struct Obj0x80047aa0 {
    char pad0[0x50];
    int x50;
} Obj0x80047aa0;
extern void func_80047B60();

void func_80047AA0(Obj0x80047aa0 *p)
{
    if (p->x50 & 1) {
        p->x50 = 0;
        func_80047B60(p);
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047AD4);

typedef struct Node0x80047b60 {
    unsigned char pad0[0x50];
    int x50;
    struct Node0x80047b60 *x54;
} Node0x80047b60;
typedef struct {
    unsigned char pad0[0x64];
    Node0x80047b60 *x64;
} Obj0x80047b60;

void func_80047B60(Obj0x80047b60 *p)
{
    Node0x80047b60 *n = p->x64;
    p->x64 = 0;
    while (n != 0) {
        n->x50 = 0;
        n = n->x54;
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047B8C);

struct Obj0x80047e0c *func_80047D5C(void)
{
    Ent0x8004758c *p;
    int i;

    i = D_800AF590 - WORK->x610;
    p = &((Ent0x8004758c *)WORK->x4E4)[WORK->x610];
    while (i-- != 0) {
        if (!(p->x50 & 1)) {
            func_80018020(p, 0x2B);
            p->x50 = 1;
            return (struct Obj0x80047e0c *)p;
        }
        p++;
    }
    return 0;
}

typedef struct Obj0x80047e0c {
    unsigned char pad0[0x58];
    unsigned char state;
    unsigned char pad59[0x98 - 0x59];
    int arg1;
    unsigned char pad9C[0xA0 - 0x9C];
    int arg0;
} Obj0x80047e0c;
typedef struct Vt0x80047e0c {
    unsigned char pad0[0x18];
    void (*fn)(Obj0x80047e0c *);
} Vt0x80047e0c;
extern Vt0x80047e0c *D_800AF124;
extern Obj0x80047e0c *func_80047D5C(void);

Obj0x80047e0c *func_80047E0C(int a0, int a1)
{
    Obj0x80047e0c *obj = func_80047D5C();

    if (obj != 0) {
        obj->state = 6;
        obj->arg0 = a0;
        obj->arg1 = a1;
        D_800AF124->fn(obj);
    }
    return obj;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047E78);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047FD0);

typedef struct Sub0x80048048 {
    int a1;
    unsigned char a2;
    unsigned char a3;
    unsigned char pad6[2];
    int a0;
    int a4;
    int a5;
} Sub0x80048048;
typedef struct Obj0x80048048 {
    unsigned char pad0[0x58];
    unsigned char state;
    unsigned char pad59[0x98 - 0x59];
    Sub0x80048048 sub;
} Obj0x80048048;

Obj0x80048048 *func_80048048(int a0, int a1, unsigned char a2, unsigned char a3, int a4, int a5)
{
    Obj0x80048048 *obj = (Obj0x80048048 *)func_80047D5C();

    if (obj != 0) {
        Sub0x80048048 *s = &obj->sub;
        obj->state = 6;
        s->a1 = a1;
        s->a2 = a2;
        s->a3 = a3;
        s->a0 = a0;
        s->a4 = a4;
        s->a5 = a5;
        D_800AF124->fn((Obj0x80047e0c *)obj);
    }
    return obj;
}

typedef struct Ent0x800480f0 {
    char pad0[0x40];
    SVECTOR x40;
    char pad48[0x10];
    unsigned char x58;
    char pad59[0x21];
    unsigned char x7A;
    char pad7B[0x1D];
    SVECTOR x98;
} Ent0x800480f0;
extern Ent0x8004758c *func_80047714(void);

Ent0x800480f0 *func_800480F0(unsigned char a0, SVECTOR *a1, short a2, short a3, short a4, short a5)
{
    Ent0x800480f0 *e;
    SVECTOR *v;

    e = (Ent0x800480f0 *)func_80047714();
    if (e != 0) {
        e->x40 = *a1;
        e->x7A = a0;
        e->x58 = 12;
        v = &e->x98;
        v->vx = a2;
        v->vy = a3;
        v->vz = a4;
        v->pad = a5;
    }
    return e;
}

typedef struct Sub0x800481a8 {
    char pad0[6];
    short x6;
    char pad8[4];
    int xC;
} Sub0x800481a8;
typedef struct Ent0x800481a8 {
    char pad0[0x58];
    unsigned char x58;
    char pad59[0x6E - 0x59];
    short x6E;
    char pad70[0x7A - 0x70];
    unsigned char x7A;
    char pad7B[0x98 - 0x7B];
    Sub0x800481a8 x98;
} Ent0x800481a8;
typedef struct Vt0x800481a8 {
    unsigned char pad0[0x34];
    void (*fn)(Ent0x800481a8 *);
} Vt0x800481a8;

Ent0x800481a8 *func_800481A8(int a0, short a1, short a2)
{
    Ent0x800481a8 *e;
    Sub0x800481a8 *v;

    e = (Ent0x800481a8 *)func_80047714();
    if (e != 0) {
        e->x58 = 13;
        v = &e->x98;
        if (a1 == 0) {
            e->x6E = 3;
        } else {
            e->x6E = a1;
        }
        e->x7A = 5;
        v->xC = a0;
        v->x6 = a2;
        ((Vt0x800481a8 *)D_800AF124)->fn(e);
    }
    return e;
}

typedef struct Src0x80048248 {
    unsigned char pad0[5];
    unsigned char x5;
} Src0x80048248;
typedef struct Obj0x80048248 {
    char pad0[0x82];
    short x82;
    char pad84[0x90 - 0x84];
    Src0x80048248 *x90;
    Src0x80048248 *x94;
} Obj0x80048248;
extern int func_80048274(); // was: extern void func_80048274(Obj0x80048248 *p); (T8 w12: def returns int)

void func_80048248(Obj0x80048248 *p, Src0x80048248 *s)
{
    p->x94 = s;
    p->x90 = s;
    p->x82 = s->x5;
    func_80048274(p);
}

typedef struct Src0x80048274 {
    unsigned char pad0[4];
    signed char x4;
    unsigned char x5;
    unsigned char pad6[2];
} Src0x80048274;
typedef struct Obj0x80048274 {
    char pad0[0x82];
    unsigned short x82;
    char pad84[0x90 - 0x84];
    Src0x80048274 *x90;
    Src0x80048274 *x94;
} Obj0x80048274;

int func_80048274(Obj0x80048274 *p)
{
    Src0x80048274 *s;
    Src0x80048274 *b;
    int ret = 0;
    int one = 1;

    if (p->x82 == one) {
        s = p->x94;
    } else if (p->x82 == 0) {
        b = p->x94;
        switch (b->x4 & 0xE0) {
        case 0x20:
            goto end;
        case 0:
            s = &b[(unsigned char)b->x4 & 0x1F];
            break;
        case 0x40:
            s = p->x90;
            break;
        default:
            goto end;
        }
        p->x94 = s;
        if ((p->x82 = s->x5) != one)
            goto end;
    } else {
        goto end;
    }
    ret = s->x4 >> 5;
end:
    p->x82--;
    return ret;
}

typedef struct Ent0x80048338 {
    char pad0[0x14];
    short x14;
    short pad16;
    short x18;
    short pad1a;
    short x1C;
} Ent0x80048338;
typedef struct Sub0x80048338 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x64 - 0x46];
    short x64;
    short x66;
    short x68;
    char pad6a[0x6C - 0x6A];
    short x6C;
    short x6E;
    short x70;
} Sub0x80048338;
typedef struct Obj0x80048338 {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x5A - 0x46];
    short x5A;
    char pad5c[0x64 - 0x5C];
    Sub0x80048338 *x64;
    char pad68[0x7C - 0x68];
    int x7C;
    char pad80[0x88 - 0x80];
    Ent0x80048338 **x88;
    char pad8c[0xE0 - 0x8C];
    short xE0;
    short xE2;
    short xE4;
} Obj0x80048338;
extern void func_800484F0();

void func_80048338(Obj0x80048338 *p, int idx, int a2, int a3, int a4)
{
    Sub0x80048338 *s = p->x64;
    Ent0x80048338 *q;
    short *e;
    unsigned short v;

    v = s->x40;
    p->x42 = -(p->x5A * 1000);
    p->x40 = v;
    p->x44 = s->x44;
    p->xE0 = s->x6C;
    p->xE2 = s->x6E;
    p->xE4 = s->x70;
    p->x7C = 0;
    if (idx >= 0) {
        q = p->x88[idx];
    } else {
        q = (Ent0x80048338 *)idx;
    }
    e = &q->x14;
    s->x64 = e[2];
    s->x66 = s->x42 - p->x42;
    s->x68 = e[4];
    func_800484F0(p, idx, a2, a3, a4);
}

typedef struct Ent0x8004840c {
    unsigned short a;
    unsigned short b;
} Ent0x8004840c;
typedef struct Tbl0x8004840c {
    int count;
    Ent0x8004840c e[1];
} Tbl0x8004840c;
typedef struct Obj0x8004840c {
    char pad0[0x92];
    unsigned short f92;
    char pad94[0x9C - 0x94];
    Tbl0x8004840c *tbl;
    char padA0[0xAA - 0xA0];
    short fAA;
    char padAC[0x102 - 0xAC];
    unsigned short f102;
} Obj0x8004840c;
extern void func_80044834(void *, int);

void func_8004840C(Obj0x8004840c *p, int div)
{
    Tbl0x8004840c *tbl;
    Ent0x8004840c *e;
    int n;

    tbl = p->tbl;
    if (tbl == 0 || (p->f102 & 0x10)) {
        return;
    }
    n = tbl->count;
    e = &tbl->e[n];
    while (n != 0) {
        e--;
        n--;
        if ((e->a * 2) / div == (p->f92 * 2) / div + p->fAA) {
            func_80044834(p, e->b);
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800484F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800487B8);

typedef struct Sv0x80048ae0 {
    short x, y, z, pad;
} Sv0x80048ae0;
typedef struct Key0x80048ae0 {
    short x0;
    short t;
    short v[1];
} Key0x80048ae0;
typedef struct Node0x80048ae0 {
    char pad0[0x48];
    Sv0x80048ae0 x48;
    char pad50[4];
    struct Node0x80048ae0 *next;
    char pad58[0xC];
    short x64[3];
} Node0x80048ae0;
typedef struct Obj0x80048ae0 {
    char pad0[0x64];
    Node0x80048ae0 *x64;
    unsigned int x68;
    char pad6C[0xC];
    Key0x80048ae0 *x78;
    Key0x80048ae0 *x7C;
    Key0x80048ae0 *x80;
    Key0x80048ae0 *x84;
    char pad88[8];
    unsigned short x90;
    short pad92;
    short x94;
    short pad96;
    short x98;
    unsigned short x9A;
    char pad9C[0x24];
    short xC0[3];
    char padC6[0x3C];
    unsigned short x102;
} Obj0x80048ae0;
#define SPAD_2C (SCRATCH(Sv0x80048ae0, 0x2C))
extern void func_8007AE94();
extern void func_8004ADD0();
extern void func_8004AFC8();
#define ABS0x80048ae0(x) ((x) < 0 ? -(x) : (x))
#define WRAP0x80048ae0(a, b)                     \
    if (ABS0x80048ae0((b) - (a)) > 0x800) {      \
        if ((b) - (a) > 0) (b) -= 0x1000;        \
        else (b) += 0x1000;                      \
    }

int func_80048AE0(Obj0x80048ae0 *p, int arg1)
{
    Node0x80048ae0 *node;
    short *dst;
    short *p0, *p1, *p2, *p3;
    int t0, t1, t2;
    int u, u2, u3;
    unsigned int i, j;
    int a, b, c, d;
    int m1, m2;
    int k;
    short tmp[4];
    Sv0x80048ae0 *q;
    int s;

    t1 = p->x7C->t;
    p0 = p->x78->v;
    p1 = p->x7C->v;
    t0 = p->x78->t;
    t2 = p->x80->t;
    p2 = p->x80->v;
    p3 = p->x84->v;
    u = (p->x98 << 14) / t1;
    u2 = (u * u) >> 14;
    node = p->x64;
    u3 = (u2 * u) >> 14;

    for (i = 0; i < p->x68 + 2; i++) {
        if (p->x9A) {
            dst = &SPAD_2C[i].x;
        } else {
            switch (i) {
            case 0:
                dst = node->x64;
                break;
            case 1:
                dst = p->xC0;
                break;
            default:
                dst = &node->x48.x;
                node = node->next;
                break;
            }
        }
        for (j = 0; j < 3; j++) {
            a = *p1;
            b = *p2;
            c = *p0;
            d = *p3;
            if (i >= 2) {
                a &= 0xFFF;
                b &= 0xFFF;
                c &= 0xFFF;
                d &= 0xFFF;
                WRAP0x80048ae0(a, b);
                WRAP0x80048ae0(b, c);
                WRAP0x80048ae0(c, d);
            }
            if (p->x94 == 0) {
                m1 = b - a;
            } else {
                m1 = ((b - a) * t0 + (a - c) * t1) / (t0 + t1);
            }
            if (p->x94 == p->x90 - 1) {
                m2 = d - b;
            } else {
                m2 = ((d - b) * t1 + (b - a) * t2) / (t1 + t2);
            }
            k = a * 3;
            k = b * 3 - k - m1 * 2 - m2;
            dst[j] = (((((a - b) * 2 + m1 + m2) * u3)
                     + (k * u2)
                     + (m1 * u)) >> 14) + a;
            p0++;
            p1++;
            p2++;
            p3++;
        }
    }

    if (p->x9A) {
        s = 0x1000 / p->x9A;
        node = p->x64;
        func_8007AE94(node->x64, &SPAD_2C[0], 0x1000 - s, s, tmp);
        node->x64[0] = tmp[0];
        node->x64[1] = tmp[1];
        node->x64[2] = tmp[2];
        q = &SPAD_2C[1];
        p->xC0[0] = q->x;
        p->xC0[1] = q->y;
        p->xC0[2] = q->z;
        q = &SPAD_2C[2];
        for (; node; node = node->next, q++) {
            if (p->x102 & 0x100) {
                func_8004AFC8(&node->x48, q, &node->x48, s);
            } else {
                func_8004ADD0(&node->x48, q, &node->x48, s);
            }
        }
        p->x9A--;
    }
    p->x98 += arg1;
    return 0;
}

extern void func_80034C48(int, int, int);
extern void func_8001BF18(Unit *);

void func_80048F60(Unit *p)
{
    Unit *u;

    u = WORK->units;
    p->flags &= ~0x20;
    p->flags &= ~0x10;
    p->flags &= ~0x40;
    p->flags &= ~0x2000;
    p->flags |= 8;
    p->x174 &= ~4;
    p->x184 &= ~1;
    u->x1E8 = 0;
    func_80034C48(1, 1, 0);
    func_8001BF18(p);
}

extern void func_8003D22C();

void func_80048FF0(Unit *u, unsigned short a1, unsigned char a2, unsigned char a3)
{
    Work *w = WORK;
    Unit *p = w->units;

    if ((signed char)w->padc2a[0] == 0) {
        a2 = 1;
        a3 = 0;
    }
    if (w->xC2B == 3) {
        a2 = 1;
        a3 = 0;
        a1 >>= 1;
    } else {
        if (a2 == 2) {
            p->x48.vy = u->x48.vy;
        }
        if (a2 == 3) {
            p->x48.vy = (u->x48.vy + 0x800) & 0xFFF;
        }
    }
    p->x54 = (a3 << 24) | (a2 << 8) | 2;
    p->x174 = 1;
    p->x176 = a1;
    p->x1D8 = u;
    func_8003D22C();
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800490AC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049134);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004916C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004920C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004924C);

typedef struct Node0x8004927c {
    char pad0[0x54];
    struct Node0x8004927c *next;
    char pad58[0x64 - 0x58];
    unsigned short x64;
    unsigned short x66;
    unsigned short x68;
} Node0x8004927c;
typedef struct Vec0x8004927c {
    unsigned short x, y, z, pad;
} Vec0x8004927c;
typedef struct Snap0x8004927c {
    char pad0[0x64];
    Node0x8004927c *head;
    char pad68[0x78 - 0x68];
    int x78, x7C, x80, x84, x88, x8C;
    unsigned short x90, x92, x94, x96, x98, x9A;
    int x9C, xA0, xA4;
    char padA8[4];
    unsigned char xAC;
    char padAD;
    unsigned char xAE, xAF;
    int xB0, xB4, xB8, xBC;
    unsigned short xC0, xC2, xC4;
    char padC6[2];
    unsigned short xC8, xCA, xCC;
    char padCE[2];
    unsigned short xD0, xD2, xD4;
    char padD6[0x102 - 0xD6];
    unsigned short x102;
    char pad104[0x10E - 0x104];
    unsigned short x10E;
    char pad110[0x260 - 0x110];
} Snap0x8004927c;
extern Vec0x8004927c D_800B3830[][15];
extern Snap0x8004927c D_800B3370[];

void func_8004927C(int idx)
{
    Vec0x8004927c *v = D_800B3830[idx];
    Snap0x8004927c *dst = &D_800B3370[idx];
    Snap0x8004927c *src = (Snap0x8004927c *)WORK->units;
    Node0x8004927c *n;

    for (n = src->head; n != 0; n = n->next) {
        v->x = n->x64;
        v->y = n->x66;
        v->z = n->x68;
        v++;
    }
    dst->x78 = src->x78;
    dst->x7C = src->x7C;
    dst->x80 = src->x80;
    dst->x84 = src->x84;
    dst->x88 = src->x88;
    dst->x8C = src->x8C;
    dst->x90 = src->x90;
    dst->x92 = src->x92;
    dst->x94 = src->x94;
    dst->x96 = src->x96;
    dst->xC0 = src->xC0;
    dst->xC2 = src->xC2;
    dst->xC4 = src->xC4;
    dst->xAC = src->xAC;
    dst->x102 = src->x102;
    dst->x98 = src->x98;
    dst->x9A = src->x9A;
    dst->x9C = src->x9C;
    dst->xA0 = src->xA0;
    dst->xA4 = src->xA4;
    dst->xB0 = src->xB0;
    dst->xB4 = src->xB4;
    dst->xB8 = src->xB8;
    dst->xBC = src->xBC;
    dst->xC8 = src->xC8;
    dst->xCA = src->xCA;
    dst->xCC = src->xCC;
    dst->xAE = src->xAE;
    dst->xAF = src->xAF;
    dst->xD0 = src->xD0;
    dst->xD2 = src->xD2;
    dst->xD4 = src->xD4;
    dst->x10E = src->x10E;
}

typedef struct Node0x8004948c {
    char pad0[0x54];
    struct Node0x8004948c *next;
    char pad58[0x64 - 0x58];
    unsigned short x64;
    unsigned short x66;
    unsigned short x68;
} Node0x8004948c;
typedef struct Vec0x8004948c {
    unsigned short x, y, z, pad;
} Vec0x8004948c;
typedef struct Snap0x8004948c {
    char pad0[0x64];
    Node0x8004948c *head;
    char pad68[0x78 - 0x68];
    int x78, x7C, x80, x84, x88, x8C;
    unsigned short x90, x92, x94, x96, x98, x9A;
    int x9C, xA0, xA4;
    char padA8[4];
    unsigned char xAC;
    char padAD;
    unsigned char xAE, xAF;
    int xB0, xB4, xB8, xBC;
    unsigned short xC0, xC2, xC4;
    char padC6[2];
    unsigned short xC8, xCA, xCC;
    char padCE[2];
    unsigned short xD0, xD2, xD4;
    char padD6[0x102 - 0xD6];
    unsigned short x102;
    char pad104[0x10E - 0x104];
    unsigned short x10E;
    char pad110[0x260 - 0x110];
} Snap0x8004948c;
extern Vec0x8004927c D_800B3830[][15];
extern Snap0x8004927c D_800B3370[];

void func_8004948C(int idx)
{
    Vec0x8004948c *v = (Vec0x8004948c *)D_800B3830[idx];
    Snap0x8004948c *src = &D_800B3370[idx];
    Snap0x8004948c *dst = (Snap0x8004948c *)WORK->units;
    Node0x8004948c *n;

    for (n = src->head; n != 0; n = n->next) {
        n->x64 = v->x;
        n->x66 = v->y;
        n->x68 = v->z;
        v++;
    }
    dst->x78 = src->x78;
    dst->x7C = src->x7C;
    dst->x80 = src->x80;
    dst->x84 = src->x84;
    dst->x88 = src->x88;
    dst->x8C = src->x8C;
    dst->x90 = src->x90;
    dst->x92 = src->x92;
    dst->x94 = src->x94;
    dst->x96 = src->x96;
    dst->xC0 = src->xC0;
    dst->xC2 = src->xC2;
    dst->xC4 = src->xC4;
    dst->xAC = src->xAC;
    dst->x102 = src->x102;
    dst->x98 = src->x98;
    dst->x9A = src->x9A;
    dst->x9C = src->x9C;
    dst->xA0 = src->xA0;
    dst->xA4 = src->xA4;
    dst->xB0 = src->xB0;
    dst->xB4 = src->xB4;
    dst->xB8 = src->xB8;
    dst->xBC = src->xBC;
    dst->xC8 = src->xC8;
    dst->xCA = src->xCA;
    dst->xCC = src->xCC;
    dst->xAE = src->xAE;
    dst->xAF = src->xAF;
    dst->xD0 = src->xD0;
    dst->xD2 = src->xD2;
    dst->xD4 = src->xD4;
    dst->x10E = src->x10E;
}

typedef struct Sv0x8004969c {
    short x, y, z, pad;
} Sv0x8004969c;
typedef struct Node0x8004969c {
    char pad0[0x48];
    Sv0x8004969c x48;
    char pad50[4];
    struct Node0x8004969c *next;
    char pad58[0xC];
    Sv0x8004969c x64;
} Node0x8004969c;
typedef struct Obj0x8004969c {
    char pad0[0x64];
    Node0x8004969c *x64;
    unsigned int x68;
    char pad6C[0xC0 - 0x6C];
    Sv0x8004969c xC0;
    Sv0x8004969c xC8;
    char padD0[0x102 - 0xD0];
    unsigned short x102;
} Obj0x8004969c;
#define SPAD0x8004969c (SCRATCH(Sv0x8004969c, 0x2C))
extern void func_800484F0(Unit *, int, int, int, int);
extern void func_8004927C(int);
extern void func_8004948C(int);
extern void func_8007AE94();
extern void func_8004ADD0();
extern void func_8004AFC8();

void func_8004969C(int a0, int a1, int a2, int a3, int a4, int a5)
{
    Obj0x8004969c *p;
    Node0x8004969c *n;
    Node0x8004969c *m;
    Sv0x8004969c *dst;
    int s;
    int i;
    int j;

    p = (Obj0x8004969c *)WORK->units;
    func_8004927C(1);
    func_800484F0((Unit *)p, a0, a2, a3 | 0x80000000, a4);
    func_8004927C(0);
    m = p->x64;
    j = p->x68 + 2;
    dst = &SPAD0x8004969c[j];
    dst->x = p->xC0.x;
    dst->y = p->xC0.y;
    dst->z = p->xC0.z;
    dst++;
    dst->x = p->xC8.x;
    dst->y = p->xC8.y;
    dst->z = p->xC8.z;
    dst++;
    dst->x = m->x64.x;
    dst->y = m->x64.y;
    dst->z = m->x64.z;
    dst++;
    for (; m; m = m->next, dst++) {
        dst->x = m->x48.x;
        dst->y = m->x48.y;
        dst->z = m->x48.z;
    }
    func_8004948C(1);
    func_800484F0((Unit *)p, a1, a2, a3 | 0x80000000, a4);
    func_8004927C(1);
    s = 0x1000 - a5;
    n = p->x64;
    i = p->x68 + 2;
    dst = &SPAD0x8004969c[i];
    func_8007AE94(dst, &p->xC0, s, a5, &p->xC0);
    dst++;
    func_8007AE94(dst, &p->xC8, s, a5, &p->xC8);
    dst++;
    func_8007AE94(dst, &n->x64, s, a5, &n->x64);
    dst++;
    for (; n; n = n->next, dst++) {
        if (p->x102 & 0x100) {
            func_8004AFC8(dst, &n->x48, &n->x48, a5);
        } else {
            func_8004ADD0(dst, &n->x48, &n->x48, a5);
        }
    }
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049900);

extern int func_8007AC34(int, int);
extern void func_80079AA4(SVECTOR *r, MATRIX *m);
extern void func_80079A64();
extern void func_800791F4();

SVECTOR *func_80049C60(MATRIX *m, SVECTOR *out)
{
    MATRIX tmp;
    SVECTOR rot;

    out->vy = -func_8007AC34(-m->m[0][2], m->m[2][2]);
    out->vz = -func_8007AC34(-m->m[1][0], m->m[1][1]);
    rot.vx = 0;
    rot.vz = 0;
    rot.vy = out->vy;
    func_80079AA4(&rot, &tmp);
    func_80079A64(&tmp, &tmp);
    func_800791F4(m, &tmp, &tmp);
    out->vx = -func_8007AC34(tmp.m[1][2], tmp.m[2][2]);
    return out;
}

void func_80049D10(void)
{
    WORK->xC48 = WORK->x3DC;
    WORK->xC4A = WORK->x3DE;
    WORK->xC4C = WORK->x3E0;
    WORK->xC4E = WORK->x3E2;
    *(Blk0x800598e0 *)((char *)WORK + 0x7C8) = *(Blk0x800598e0 *)((char *)WORK + 0xC28);
}

void func_80049DAC(void)
{
    *(Blk0x800598e0 *)((char *)WORK + 0xC28) = *(Blk0x800598e0 *)((char *)WORK + 0x7C8);
    WORK->x3DC = WORK->xC48;
    WORK->x3DE = WORK->xC4A;
    WORK->x3E0 = WORK->xC4C;
    WORK->x3E2 = WORK->xC4E;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049E44);

extern int func_8004A2C4(void *p, int arg);

short func_80049EA0(void *p, int arg, short ang)
{
    short d;

    d = (func_8004A2C4(p, arg) - ang) & 0xFFF;
    if (d & 0x800) {
        d |= 0xF000;
    }
    return d;
}

s16 func_80049EEC(s16 arg0, s16 arg1) {
    s16 var_a1;

    var_a1 = (arg1 - arg0) & 0xFFF;
    if (var_a1 & 0x800) {
        var_a1 |= ~0xFFF;
    }
    return var_a1;
}

typedef struct {
    char pad0[0x4A];
    short v4A;
} Obj80049F24;
extern int func_8004A2C4(void *p, int arg);
extern short func_8003DD34(int a, int b, int c);

void func_80049F24(Obj80049F24 *obj, short a1, int a2)
{
    unsigned short t;

    t = func_8004A2C4((char *)obj + 0x40, a2);
    obj->v4A += func_8003DD34(obj->v4A, a1, t);
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049F7C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049FD0);

extern int func_8004A0F8();
extern int func_80078E14(int);
extern int func_8007AC34(int, int);
extern int func_80078844(int);
extern int func_80078914(int);

void func_8004A050(SVECTOR *a, SVECTOR *b, unsigned short dist, SVECTOR *out)
{
    short ang;
    int r;

    r = func_80078E14(func_8004A0F8(b, a));
    ang = func_8007AC34(b->vy - a->vy, r);
    out->vx = 0;
    out->vy = (func_80078844(ang) * dist) >> 12;
    out->vz = (func_80078914(ang) * dist) >> 12;
}

int func_8004A0F8(V3s_s800436C8 *a, V3s_s800436C8 *b)
{
#include "../shared/func_800436C8.inc.c"
}

extern int func_80078E14(int);

unsigned int func_8004A144(unsigned int a)
{
    unsigned int x = func_80078E14(a);

    if (x != 0) {
        x = (x + a / x) >> 1;
    }
    return x;
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004A194);


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


