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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046470);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004653C);

int func_800469D0(void)
{
#include "../shared/func_800469D0.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800469D8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046BBC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046C58);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046F6C);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80046FC0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047168);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800471CC);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800472F4);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800474DC);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047714);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047814);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800478B8);

void func_80047958(int *a0)
{
#include "../shared/func_80047958.inc.c"
}

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047960);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800479FC);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80047D5C);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80048048);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800480F0);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_800481A8);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80048338);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004840C);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80048FF0);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049C60);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049D10);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049DAC);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_80049EEC);

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

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800458F0", func_8004A050);

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


