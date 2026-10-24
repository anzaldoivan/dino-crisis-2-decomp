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

typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    int *x84;
} Obj0x8006351c;
extern int D_800B49DC[];

int func_8006351C(Obj0x8006351c *o)
{
    int *p = D_800B49DC;
    Unit *u = &WORK->units[WORK->x61C + 1];
    unsigned int i;

    for (i = 0; i < D_800AF11C - WORK->x61C - 1; i++, u++, p++) {
        {
            if (*o->x84 == 1) {
                u->flags = *p;
            } else {
                *p = u->flags;
                u->flags = 0;
            }
        }
    }
    o->pc += 2;
    return 0;
}

int func_800635E8(int *arg0)
{
    WORK->x220 = 0xFFFF;
    *arg0 += 2;
    return 0;
}

#define SHARED_F0 func_80030DDC
int func_80063610(ScriptCtx_s800631D8 *ctx)
{
#include "../shared/func_800631D8.inc.c"
}

typedef struct {
    int w0;
    int w4;
} Src0x80063654;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x80063654 *src;
    char pad88[0x9F - 0x88];
    signed char x9F;
} Obj0x80063654;
extern char *func_800474B4(int a, int b);

int func_80063654(Obj0x80063654 *o)
{
    Unit *u = &WORK->units[o->x9F];
    char *p = func_800474B4(u->id, o->src->w4);

    if (o->src->w0 == 1) {
        *(int *)(p + 0x50) &= ~0x10000000;
    } else {
        *(int *)(p + 0x50) |= 0x10000000;
    }
    o->pc += 2;
    return 0;
}

typedef struct {
    int w0;
    unsigned short x4;
    char pad6[0x8 - 0x6];
    unsigned short x8;
    char padA[0xC - 0xA];
    unsigned short xC;
} Src0x80063700;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x80063700 *src;
} Obj0x80063700;
extern int func_80034CD8(int, int);
extern void func_80041A68(int idx);
extern int func_8007B380(int);
extern int func_8007B62C(Box0x8002243c *, void *);

int func_80063700(Obj0x80063700 *o)
{
    Box0x8002243c r;

    if (func_80034CD8(9, 0x33)) {
        func_80041A68(o->src->w0);
        r.x = o->src->xC;
        r.y = o->src->x8;
        r.w = 0x80;
        r.h = 0x100;
        func_8007B62C(&r, D_801C1500);
        func_8007B380(0);
        r.x = 0;
        r.y = o->src->x4;
        r.w = 0x100;
        r.h = 1;
        func_8007B62C(&r, D_801D1500);
        func_8007B380(0);
    }
    o->pc += 2;
    return 0;
}

int func_800637D0(int *a0)
{
#include "../shared/func_8005ECB4.inc.c"
}

typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    int *src;                                                         
} Obj0x800637e8;
typedef struct {
    int pad0[0x40 / 4];
    short x40;
    short x42;
    short x44;
    char pad46[0x60 - 0x46];
} Head0x800637e8;
extern Unit *func_8005E940();
extern void func_80044834(void *, int);

int func_800637E8(Obj0x800637e8 *o)
{
    Unit *u = func_8005E940();
    Head0x800637e8 h;
    Head0x800637e8 *p;

    if (u >= WORK->units && u <= &WORK->units[D_800AF11C - 1]) {
        p = (Head0x800637e8 *)func_800474B4(u->id, 0);
        h = *(Head0x800637e8 *)u;
        h.x40 = p->x40;
        h.x42 = p->x42;
        h.x44 = p->x44;
        u = (Unit *)&h;
    }
    func_80044834(u, *o->src);
    o->pc += 2;
    return 0;
}

#define SHARED_F0 func_80044AB4
int func_800638F8(ScriptCtx_s800631D8 *ctx)
{
#include "../shared/func_800631D8.inc.c"
}

#define SHARED_F0 func_80044B80
int func_8006393C(ScriptCtx_s800610A0 *ctx)
{
#include "../shared/func_800610A0.inc.c"
}

extern int func_80034CD8(int, int);

int func_80063988(Obj0x8006335c *o)
{
    int ret;
    int t;

    if (*o->src != 0) {
        t = func_80034CD8(2, 0);
        ret = 1;
        if (t == 0) {
            goto end;
        }
    } else {
        if (func_80034CD8(2, 0) != 0) {
            ret = 1;
            goto end;
        }
        if (func_80034CD8(2, 2) != 0) {
            ret = 1;
            goto end;
        }
    }
    o->pc += 2;
    ret = 0;
end:
    return ret;
}

typedef struct {
    union {
        int w;
        unsigned short h;
    } f0;
    unsigned short f4;
} Src0x80063a10;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src0x80063a10 *src;
} Obj0x80063a10;
extern int func_80034CD8(int, int);
extern void func_8001D9E4(void);
extern void func_8001E4A0(int, int);

int func_80063A10(Obj0x80063a10 *o)
{
    Src0x80063a10 *s;

    if (func_80034CD8(2, 2) != 0) {
        return 1;
    }
    s = o->src;
    if (s->f0.w == -1) {
        func_8001D9E4();
    } else {
        func_8001E4A0(s->f4, s->f0.h);
    }
    o->pc += 2;
    return 0;
}

typedef struct {
    unsigned short h0;
    char pad02[2];
    unsigned short h4;
    char pad06[2];
    short h8;
    char pad0A[3];
    unsigned char bD;
} Arg0x80063a8c;
typedef struct {
    int pc;
    char pad04[0x80];
    Arg0x80063a8c *arg;
} Ctx0x80063a8c;
extern void func_80045B8C();

int func_80063A8C(Ctx0x80063a8c *ctx)
{
    func_80045B8C(ctx->arg->bD, ctx->arg->h8, ctx->arg->h4, ctx->arg->h0);
    ctx->pc += 2;
    return 0;
}

typedef struct {
    char pad0[0x22];
    short x22;
    char pad24[4];
    unsigned char x28;
    unsigned char x29;
    char pad2a[0xEC - 0x2A];
} Ent0x80063adc;
typedef struct {
    int x0;
    char pad4[0x80];
    unsigned short *x84;
} Obj0x80063adc;
extern Ent0x80063adc D_800B2F28[];
extern void func_80045460();
extern unsigned short *func_800458F0(int a, int b);
extern void func_80045A20(int, int);
extern void func_80045C18(int arg0);

int func_80063ADC(Obj0x80063adc *obj)
{
    unsigned short *p = obj->x84;
    unsigned short flags = p[2];
    unsigned short v = p[0];
    unsigned short i;
    int mask;
    Ent0x80063adc *e;

    if (flags & 1) {
        func_80045A20(v >> 8, v & 0xFF);
    }
    if (flags & 2) {
        func_80045C18(v >> 8);
    }
    if (flags & 4) {
        Ent0x80063adc *base = D_800B2F28;
        base[v >> 8].x28 = 0x80;
        base[v >> 8].x29 = 3;
        base[v >> 8].x22 = 0;
    }
    if (flags & 0x10) {
        mask = func_800458F0(WORK->xC2D, WORK->xC2C)[1];
        for (i = 0; i < 8; i++) {
            if ((mask >> i) & 1) {
                e = &D_800B2F28[i & 3];
                e->x28 = 0x80;
                e->x29 = 2;
                e->x22 = 0;
            }
        }
    }
    if (flags & 8) {
        func_80045460(func_800458F0(WORK->xC2D, WORK->xC2C)[1]);
    }
    obj->x0 += 2;
    return 0;
}

typedef struct {
    unsigned short f0;
    unsigned short f2;
    unsigned short f4;
    unsigned short f6;
    unsigned short f8;
} Src80063C50;
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    Src80063C50 *src;
} Obj80063C50;
extern unsigned short *func_800458F0(int a, int b);

int func_80063C50(Obj80063C50 *o)
{
    unsigned short v = o->src->f8;
    unsigned short *r = func_800458F0(v >> 8, v & 0xFF);
    r[0] = o->src->f4;
    r[1] = o->src->f0;
    o->pc += 2;
    return 0;
}

extern Elem28 D_800B4A68[];

void func_80063CC4(void)
{
    unsigned int i;

    WORK->x5FC = D_800B4A68;
    for (i = 0; i < 32; i++) {
        D_800B4A68[i].b0 = 0;
    }
}

typedef struct {
    char pad0[0x54];
    unsigned char x54;
} Unit0x80063cfc;
typedef struct {
    char pad0[0x4E0];
    Unit0x80063cfc *units;
    char pad4e4[0x6C8 - 0x4E4];
    char x6C8[8];
} Work0x80063cfc;
#define WORK0x80063cfc (*SCRATCH(Work0x80063cfc *, 0))
extern void func_80018020(void *, int);
extern void func_80034C48(int, int, int);
extern void func_80063D78(void);

void func_80063CFC(void)
{
    Work0x80063cfc *w = WORK0x80063cfc;
    Unit0x80063cfc *u = w->units;

    func_80018020(w->x6C8, 8);
    if (u->x54 == 1) {
        func_80063D78();
    }
    func_80034C48(2, 1, 0);
    func_80034C48(2, 13, 0);
    func_80034C48(2, 10, 0);
}

extern void (*D_8008A3B0[])(Elem28 *);
extern unsigned short D_800B4F68[3];
extern unsigned short D_800B4F70;
extern int func_80034CD8(int, int);
extern int func_8004A378(void *, void *);
extern SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
extern int func_80079AA4(SVECTOR *r, MATRIX *m);

void func_80063D78(void)
{
    Unit0x800640b8 *u;
    Elem0x800640b8 *e;
    Elem0x800640b8 *end;
    SVECTOR v;
    SVECTOR *p;
    int mode;
    short t;
    int k;

    mode = WORK->xC50 >> 8;
    end = (Elem0x800640b8 *)WORK->x5FC + 32;
    u = (Unit0x800640b8 *)WORK->units;
    if (u->b55 == 10 && (mode == 3 || mode == 6)) {
        mode = 1;
    } else {
        mode = 0;
    }
    for (e = (Elem0x800640b8 *)WORK->x5FC; e < end; e++) {
        if (!(e->b0 & 1)) {
            continue;
        }
        if ((e->b0 & 0xC) != 0xC) {
            t = u->x5A;
        } else {
            t = (short)D_800B4F68[1] / -1000;
        }
        if (e->b2 != t) {
            continue;
        }
        if (mode && !(e->b0 & 0x40)) {
            continue;
        }
        switch (e->b0 & 0xC) {
        case 0:
            if (func_80034CD8(1, 0)) {
                continue;
            }
            if (!func_80034CD8(2, 1)) {
                continue;
            }
            break;
        case 4:
            break;
        case 8:
            if (func_80034CD8(1, 0)) {
                continue;
            }
            if (!func_80034CD8(2, 13)) {
                continue;
            }
            break;
        case 0xC:
            if (!func_80034CD8(2, 10)) {
                continue;
            }
            switch (k = e->b0 & 0xF0) {
            case 0:
                break;
            case 0x10:
                if (D_800B4F70 != k) {
                    continue;
                }
                break;
            case 0x20:
                if (D_800B4F70 != k) {
                    continue;
                }
                break;
            case 0x30:
                if ((unsigned char)D_800B4F70 != k) {
                    continue;
                }
                break;
            }
            break;
        }
        if ((e->b0 & 0xC) == 0xC) {
            p = (SVECTOR *)D_800B4F68;
        } else if (e->b0 & 2) {
            v.vx = 0;
            v.vy = 0;
            v.vz = 400;
            func_80079AA4(&u->x48, &u->m);
            func_80079704(&u->m, &v, &v);
            v.vx += u->x40.vx;
            v.vy += u->x40.vy;
            v.vz += u->x40.vz;
            p = &v;
        } else {
            p = &u->x40;
        }
        if (func_8004A378(p, e->x4)) {
            D_8008A3B0[e->b1]((Elem28 *)e);
            if (!(e->b0 & 4)) {
                return;
            }
        }
    }
}

extern int func_8004A378(void *, void *);
extern SVECTOR *func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
extern int func_80079AA4(SVECTOR *r, MATRIX *m);

int func_800640B8(unsigned char arg)
{
    Unit0x800640b8 *u;
    Elem0x800640b8 *e;
    Elem0x800640b8 *end;
    SVECTOR v;
    SVECTOR *p;
    int mode;

    end = (Elem0x800640b8 *)WORK->x5FC + 32;
    mode = WORK->xC50 >> 8;
    u = (Unit0x800640b8 *)WORK->units;
    if (u->b55 == 10) {
        if (mode == 3) {
            return 0;
        }
        if (mode == 6) {
            return 0;
        }
    }
    for (e = (Elem0x800640b8 *)WORK->x5FC; e < end; e++) {
        if (!(e->b0 & 1)) {
            continue;
        }
        if (e->b1 != 9) {
            continue;
        }
        if (e->b2 != u->x5A) {
            continue;
        }
        if (e->b0 & 2) {
            v.vx = 0;
            v.vy = 0;
            v.vz = 400;
            func_80079AA4(&u->x48, &u->m);
            func_80079704(&u->m, &v, &v);
            v.vx += u->x40.vx;
            v.vy += u->x40.vy;
            v.vz += u->x40.vz;
            p = &v;
        } else {
            p = &u->x40;
        }
        if (func_8004A378(p, e->x4) && e->b19 == arg) {
            return 1;
        }
    }
    return 0;
}

extern int func_8004A378(void *, void *);

Elem28 *func_80064238(short x, short y, short z)
{
    SVECTOR v;
    Elem28 *e;
    Elem28 *end;
    Unit *u;
    unsigned char b0;

    Elem28 *base;

    base = WORK->x5FC;
    end = base + 32;
    for (e = base; e < end; e++) {
        b0 = e->b0;
        u = WORK->units;
        if ((b0 & 1) && e->pad[1] == u->x5A && (b0 & 0xC)) {
            v.vx = x;
            v.vy = y;
            v.vz = z;
            if (func_8004A378(&v, &e->pad[3]) != 0) {
                return e;
            }
        }
    }
    return 0;
}

typedef struct {
    unsigned char x0;
    char pad1[0x14 - 0x1];
    char x14[0x20 - 0x14];
    unsigned char x20;
    unsigned char x21;
} Obj0x80064314;
extern void func_8001CED4(int);
extern int func_80034CD8(int, int);
extern void func_80034D88();
extern void func_8005AD28();
extern int func_80065D0C();
extern void func_800288AC();
extern char *D_8008A470[];
extern int D_800B4F74;

void func_80064314(Obj0x80064314 *obj)
{
    unsigned char f = obj->x0;
    int b = obj->x21;
    int s = obj->x20;
    unsigned char *p;
    int i;

    if ((f & 0xC) != 0xC && (f & 0x80)) {
        WORK->x4C6 = 1;
    }
    if (s == 0 || func_80034CD8(8, b) != 0) {
        D_800B4F74 = (int)obj->x14;
        for (i = 0; i < 32; i++) {
            WORK->x5FC[i].b0 = 0;
        }
        func_80034D88(-1, 0, 0);
        func_8005AD28();
        func_8001CED4((int)func_800288AC);
    } else {
        p = ((unsigned char *(*)())func_80065D0C)(0, 0, D_8008A470[s], 3);
        if ((obj->x0 & 0xC) == 8) {
            *p |= 2;
        }
    }
}

typedef struct Obj0x80064434 {
    char pad0[0x14];
    signed char x14;
    char pad15[3];
    short x18;
} Obj0x80064434;
extern void func_8005E854(int, int);

void func_80064434(Obj0x80064434 *obj)
{
    func_8005E854(obj->x14, obj->x18);
}

extern void func_80034C48(int, int, int);
extern unsigned char D_800B49D4;


extern int func_80034CD8(int, int);
extern int func_80065D0C(int, int, char *, int);
extern char D_8008EB88[];
extern char D_8008E98E[];


extern int D_800B4F74;


extern void func_80065EC8(void);
extern void func_80018020(void *, int);
extern void func_80065F24(int, int);
extern void func_80065EFC(void);
extern void func_80034C48(int, int, int);
extern S800B53A8 D_800B53A8;



extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;




#define SHARED_D0 D_8008F780




#define SHARED_D0 D_8008F78C

#define SHARED_F0 func_800661FC

