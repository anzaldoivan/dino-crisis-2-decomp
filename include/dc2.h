/* dc2.h — canonical shared shapes (T2, Phase 1.6). A struct used by C units is defined here once; units include
 * this header and never redefine it (tools/types_check.py). Each field cites the instruction proving its width and
 * signedness (objdump of the byte-identical unit object); unproven bytes are padXX keeping offsets. */
#ifndef DC2_H
#define DC2_H

#include "common.h"

/* psx_bin_st6 func_800D5C40: stride 12 proven by `addiu a2,a2,12` in the loop. */
typedef struct {
    char pad0[8];               /* a, b: never accessed */
    int c;                      /* sw/lw +8 (lw -12(a2) = previous entry's c) */
} Entry;

/* psx_bin_st6 func_800D5C40 (arg a0). Total size unproven. */
typedef struct {
    char pad0[0x98];
    int x98;                    /* lw 0x98(a0) */
    char pad9c[2];              /* was short; never accessed */
    unsigned short x9e[8];      /* lhu 0(a1), a1 = a0+0x9e, step 2, 8 iterations */
} Obj;

/* PsyQ libgte shapes (LIBGTE.H layout), passed to the gte wrappers by slus_012_79 and psx_bin_st1 units. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

/* slus_012_79 func_80037824 and its continuation units: D_800893D0. */
typedef struct {
    short a;
    short b;
} Pair;

/* slus_012_79 D_800B4A68[32] (func_80063CC4 clears b0 of each): stride 0x28. */
typedef struct {
    unsigned char b0;
    unsigned char pad[0x27];
} Elem28;

/* slus_012_79 D_800B53A8 (func_80065004 and the 8003DD34/800458F0/8004B8E8 units). */
typedef struct {
    unsigned char pad0[2];
    unsigned char unk2;
    unsigned char pad3[0xC];
    unsigned char unkF;
    unsigned char b10;          /* func_80065F24 (T8 w06) */
    unsigned char b11;          /* func_80065F24 (T8 w06) */
    char pad12[2];
    int x14;                    /* func_8006548C: lw 0x14 */
    unsigned char b18;          /* func_8006548C: lbu 0x18, andi 1 */
} S800B53A8;

/* slus_012_79 script context (func_8005F088 and the 800458F0/8004B8E8 units): arg at 0x84, b9E/b9F byte stores. */
typedef struct {
    int pc;
    char pad04[0x80];
    unsigned char *arg;
    char pad88[0x16];
    unsigned char b9E;
    unsigned char b9F;
} Ctx;

/* slus_012_79 func_80032908 operand (func_8003D2D4, func_8003D58C): four halfword stores, xz plane. */
typedef struct { short x0, z0, x1, z1; } Seg;

/* slus_012_79 area table at Work+0x5EC (func_8003D2D4, func_8003D58C): lbu 0 = count, offs[] words from +8. */
typedef struct {
    unsigned char count;
    char pad1[7];
    int offs[1];
} AreaTbl;

/* slus_012_79 func_80037E18: size 0x260 proven by `addiu a2,a2,608` and the n*19*32 bound. */
typedef struct {
    char pad0[0x20];            /* was pad0[0x40] */
    MATRIX x20;                 /* func_80029154 (T8 w27) */
    short x40;                  /* func_8006AC8C (T8 w07) */
    char pad42[2];
    short x44;                  /* func_8006AC8C (T8 w07) */
    char pad46[0x48 - 0x46];    /* was pad46[0x50 - 0x46] */
    SVECTOR x48;                /* func_80029154 (T8 w27) */
    int flags;                  /* lw 0x50(a2); andi 1 */
    char pad54[0x64 - 0x54];
    int id;                     /* psx_bin_st1 func_800D9000: lw 0x64 of WORK->units, passed as arg 0 */
    int x68;                    /* func_80037C9C (T8 w28): lw 0x68 loop count */
    char pad6c[0x7C - 0x6C];    /* was pad68[0x7C - 0x68] */
    int x7C;                   /* func_80060F2C (T8 w07) */
    char pad80[0xFD - 0x80];
    unsigned char side;         /* lbu 0xFD(a2); sb 0xFD(t0) */
    unsigned char xFE;          /* func_80037C9C (T8 w28): lbu/sb 0xFE */
    char padff[0x108 - 0xFF];   /* was padfe[0x108 - 0xFE] */
    unsigned short x108;        /* func_80023D8C (T8 w08) */
    char pad10a[0x174 - 0x10A];
    unsigned short x174;        /* func_80048F60: lhu/andi ~4/sh 0x174 */
    char pad176[0x184 - 0x176];
    int x184;                   /* func_80048F60: lw/and ~1/sw 0x184 */
    char pad188[0x1C8 - 0x188];
    short x1C8;                 /* func_8006AC8C (T8 w07) */
    char pad1ca[0x1E8 - 0x1CA];
    short x1E8;                 /* func_80048F60: sh zero,0x1E8 of WORK->units */
    char pad1ea[0x260 - 0x1EA];
} Unit;

/* slus_012_79 Work+0xC58 records (func_800544FC: lbu 1 of WORK->xC58[i], stride 12). */
typedef struct {
    unsigned char b0;
    unsigned char b1;           /* func_800544FC: lbu 1, indexes D_80019770 */
    char pad2[6];
    short x8;                   /* func_800523EC: lh 8 (T8 w07) */
    char padA[2];
} Rec12;

/* Work+0x6EC four-byte record (func_80030670, T8 w31). */
typedef struct {
    unsigned char x0;
    unsigned char x1;
    unsigned char x2;
    unsigned char x3;
} Ramp0x80030670;

/* Scratchpad-resident work block (word 0 of the scratchpad). Total size unproven. Field users in comments. */
typedef struct {
    char pad0[4];
    int x4;                     /* func_8004C960: lw 4, andi 1/3 frame counter (T8 w10) */
    void *x8;                   /* func_8004283C (T8 w06) */
    char padC[3];
    unsigned char xF;           /* func_80029678 (T8 w05) */
    char pad10[0x200 - 0x10];
    unsigned char x200;         /* func_8001D900 (T8 w08) */
    unsigned char x201;         /* func_8001D900 (T8 w08) */
    unsigned char x202;         /* func_8002F3E8 (T8 w25) */
    char pad203[1];             /* was pad202[2] */
    short x204;                 /* func_80029678 (T8 w05) */
    short x206;
    short x208;
    short x20A;
    char pad20c[0x214 - 0x20C]; /* was pad20c[0x218 - 0x20C] */
    unsigned short x214;        /* func_8002F3E8 (T8 w25) */
    unsigned short x216;        /* func_8002F3E8 (T8 w25) */
    unsigned short x218;        /* func_800418DC (T8 w03 match) */
    unsigned short x21A;        /* func_80059FB0: sh 0xFFFF */
    unsigned short x21C;        /* func_8002F3E8 (T8 w25); was pad21c[0x21E - 0x21C] */
    unsigned short x21E;        /* func_80041810 (T8 w05) */
    unsigned short x220;        /* func_800635E8: ori v0,zero,0xFFFF; sh v0,0x220(v1) */
    unsigned short x222;        /* func_80041DC0 (T8 w23) */
    char pad224[0x228 - 0x224]; /* was pad222[0x228 - 0x222] */
    int x228;                   /* func_80052634: lw v1,0(0x1F800000); lw v0,0x228(v1); addiu; sw v0,0x228(v1) */
    char pad22c[0x30C - 0x22C]; /* was pad22c[0x30E - 0x22C] */
    short x30C;                 /* func_800591B0 (T8 w15) */
    short x30E;                 /* func_8005A07C (T8 w03 match) */
    short x310;                 /* func_8005A07C (T8 w03 match) */
    short x312;                 /* func_8005A07C: switch on it, ++ */
    char pad314[0x318 - 0x314];
    char x318[0x3DC - 0x318];   /* func_80036E94: address passed to func_80037C9C; was x318[0xCE] */
    unsigned short x3DC;        /* func_80038FB4 (T8 w22): lhu */
    unsigned short x3DE;        /* func_80038FB4 (T8 w22): lhu */
    unsigned short x3E0;        /* func_80038FB4 (T8 w22): lhu */
    unsigned short x3E2;        /* func_80038FB4 (T8 w22): lhu */
    char pad3e4[0x3E6 - 0x3E4];
    unsigned char x3E6;         /* func_80036E94: lbu, compared with x3E7 */
    unsigned char x3E7;         /* func_80036E94: lbu, sb from x3E6 */
    char pad3e8[0x3EC - 0x3E8]; /* was pad3e8[0x4BA - 0x3E8] */
    unsigned char x3EC;         /* func_8002F3E8 (T8 w25) */
    char pad3ed[0x3EE - 0x3ED]; /* was pad3ed[0x4BA - 0x3ED] */
    unsigned char x3EE;         /* func_8004758C (T8 w27) */
    char pad3ef[0x4B9 - 0x3EF]; /* was pad3ef[0x4BA - 0x3EF] */
    unsigned char x4B9;         /* func_80037068 (T8 w29) */
    unsigned char x4BA;         /* func_800375D4: sb 2 / lbu, compared with 2 */
    char pad4bb[0x4BC - 0x4BB]; /* was pad4bb[0x4C7 - 0x4BB], pad4bb[0x4CC - 0x4BB] */
    unsigned short x4BC;        /* func_80037068 (T8 w29) */
    char pad4be[0x4C6 - 0x4BE]; /* was pad4be[0x4C7 - 0x4BE] */
    unsigned char x4C6;         /* func_8002F654: lbu, compared with 0; sb zero */
    unsigned char x4C7;         /* func_8002F3E8 (T8 w25) */
    char pad4c8[0x4CC - 0x4C8];
    void (*x4CC)(void *);       /* func_80039B7C: lw 0x4CC, jalr with a0 = obj */
    void (*x4D0)(void);         /* func_8004B4C8: lw 0x4D0, jalr, no args */
    char pad4d4[0x4D8 - 0x4D4]; /* was pad4d4[0x4DC - 0x4D4] */
    char x4D8[4];               /* func_8002F654: address passed to func_80018020 */
    struct Shot *x4DC;          /* func_8003EB20: lw 0x4DC, 12 elements stride 0x70; shape local to that fn */
    Unit *units;               /* lw 0x4E0(v0); units[0] is the player (func_8003D2D4, func_8003D58C, st1) */
    void *x4E4;                 /* func_8002CC2C, func_80023ED8: entity array, stride 0xAC (T8 w13) */
    void *x4E8;                 /* func_8005E6A4 (T8 w31) */
    char pad4ec[0x5DC - 0x4EC]; /* was pad4e8[0x5DC - 0x4E8] */
    void *x5DC;                /* func_80037C1C: sw &D_800B24F8 (T8 w10) */
    char pad5e0[0x5E4 - 0x5E0];
    int x5E4;                   /* func_8001D230: lw, arg 1 of func_8001D338 */
    char pad5e8[0x5EC - 0x5E8];
    AreaTbl *areas;             /* func_8003D2D4, func_8003D58C: lw 0x5EC */
    char pad5f0[0x5F4 - 0x5F0];
    char *x5F4;                 /* func_80064460, func_8005E450: bump cursor */
    char pad5f8[0x5FC - 0x5F8];
    Elem28 *x5FC;               /* func_80063CC4: sw of D_800B4A68 */
    char pad600[0x604 - 0x600]; /* was pad600[0x6B0 - 0x600], pad600[0x6B4 - 0x600] */
    void *x604;                 /* func_800442D4 (T8 w27) */
    char pad608[0x629 - 0x608]; /* was pad608[0x6B0 - 0x608] */
    unsigned char x629;         /* func_8002F654: sb 0xFF */
    char pad62a[0x671 - 0x62A];
    unsigned char x671;         /* func_8002F654: sb 0xFF */
    char pad672[0x6B0 - 0x672];
    short x6B0;                 /* func_8005AF1C (T8 w15) */
    char pad6b2[0x6B4 - 0x6B2];
    int x6B4;                   /* func_8005F714: lw 0x6B4 */
    int x6B8;                   /* func_8006335C, func_800633E8 (T8 w03 match) */
    char pad6bc[0x6E8 - 0x6BC];
    char x6E8;                  /* func_8004B7B8: sb */
    unsigned char x6E9;         /* func_8001DB64 (T8 w05) */
    unsigned char x6EA;         /* func_8005E450: sb 0xFF */
    unsigned char x6EB;         /* func_8002F654: sb 4; was pad6eb[0x6EC - 0x6EB], pad6eb[0x6F8 - 0x6EB] */
    Ramp0x80030670 x6EC;        /* func_80030670 (T8 w31): lbu 0..3, sb 2 */
    char pad6f0[0x6F8 - 0x6F0];
    unsigned char x6F8;         /* func_8004C0B4: sb 0xA0 */
    unsigned char x6F9;         /* func_8004C0B4: sb 0xA0 */
    unsigned char x6FA;         /* func_8004C0B4: sb 0xA0 */
    unsigned char x6FB;         /* func_8004C0B4: sb zero */
    unsigned char x6FC;         /* func_8004C0B4: sb zero */
    char pad6fd[0x712 - 0x6FD]; /* was pad6fd[0x7C6 - 0x6FD] */
    unsigned char x712;         /* func_80030670 (T8 w31): xori 1 toggle */
    char pad713[0x7C6 - 0x713];
    unsigned char x7C6;         /* func_80031084 (T8 w07) */
    unsigned char x7C7;         /* func_80031084 (T8 w07) */
    char pad7c8[0xC29 - 0x7C8];
    unsigned char xC29;         /* func_8001D134 (T8 w07) */
    unsigned char padc2a[1];    /* xC2A: func_8002F3E8 stores padc2a[0] (T8 w25), func_800526B4 reads it signed; was char */
    unsigned char xC2B;         /* func_80039654: lbu, compared with 2 */
    unsigned char xC2C;         /* func_8004587C: lbu, arg 1 of func_800458F0 */
    unsigned char xC2D;         /* func_8004587C: lbu, arg 0 of func_800458F0 */
    char padc2e[2];
    unsigned char xC30;         /* func_800523EC: lbu, &1 (T8 w07) */
    char padc31[0xC44 - 0xC31]; /* was padc31[0xC48 - 0xC31], padc31[0xC50 - 0xC31] */
    unsigned char xC44;         /* func_800292F4 (T8 w29) */
    unsigned char xC45;         /* func_800292F4 (T8 w29) */
    char padc46[0xC48 - 0xC46];
    unsigned short xC48;        /* func_8002F3E8 (T8 w25) */
    unsigned short xC4A;
    unsigned short xC4C;
    unsigned short xC4E;
    unsigned short xC50;        /* func_8003EA1C: lhu 0xC50, >> 8 indexes D_80089AB8 */
    char padc52[0xC58 - 0xC52];
    Rec12 xC58[1];              /* func_800544FC: lbu 1 of WORK+0xC58+12*i; count unproven */
    char padc64[0xD94 - 0xC64]; /* was padc64[0xE64 - 0xC64] */
    unsigned short xD94;        /* func_80038FB4 (T8 w22): lhu */
    char padd96[0xD98 - 0xD96];
    unsigned short xD98;        /* func_80038FB4 (T8 w22): lhu */
    char padd9a[0xD9C - 0xD9A];
    unsigned short xD9C;        /* func_80038FB4 (T8 w22): lhu */
    char padd9e[0xE64 - 0xD9E];
    int xE64;                   /* func_8002219C: lw, decremented, sw */
    int xE68;                   /* func_8002219C: lw */
    unsigned char xE6C[0x5C];   /* func_800615EC: sb 0 at +0xE6C+i, i<0x5C */
    char padec8[0xF24 - 0xEC8];
    short xF24;                 /* func_80041DC0 (T8 w23) */
    short xF26;                 /* func_80041DC0 (T8 w23) */
    char padf28[0x108C - 0xF28];
    signed char x108C;          /* func_8002F654, func_80031968: lb, arg 2 of func_80034C48 */
    signed char x108D;          /* func_8002F654: lb, arg 2 of func_80034C48 */
    signed char x108E;          /* func_8002F654: lb, arg 2 of func_80034C48 */
} Work;

/* Scratchpad (0x1F800000, 1 KiB): `lui 0x1f80` + offset. SCRATCH is the only home of a scratchpad address literal;
 * every scratchpad access is `*SCRATCH(T, off)` (a word) or `SCRATCH(T, off)` (a pointer into it). */
#define SCRATCH(T, off) ((T *)(0x1F800000 + (off)))

/* Scratchpad word 0: `lui 0x1f80; lw 0(v0)`. */
#define WORK (*SCRATCH(Work *, 0))

/* Scratchpad word 1: primitive packet cursor (func_8002204C: lw, stores through it, advances by 12). */
#define PKT (*SCRATCH(u32 *, 4))

/* func_8006AC8C, func_8006AF9C: 16-byte table rows, lhu 6 (T8.c465). */
typedef struct {
    char pad0[6];
    unsigned short h6;
    char pad8[8];
} Elem0x8006ac8c;

/* func_8006AC8C, func_8006AF9C: view of WORK->x318, lh 0x22, lw 0x40, lbu 0x4C (T8.c465). */
typedef struct {
    char pad0[0x22];
    short h22;
    char pad24[0x40 - 0x24];
    Elem0x8006ac8c *x40;
    char pad44[0x4C - 0x44];
    unsigned char b4C;
} Obj0x8006ac8c;

/* func_80066C20: lb 3 indexes D_8008F7F0 (T8.c465). */
typedef struct Obj80066C20 {
    unsigned char pad0[3];
    signed char state;
} Obj80066C20;

/* func_8006C538: lb 3 indexes D_8008F894 (T8.c465). */
typedef struct Obj0x8006c538 {
    unsigned char pad0[3];
    signed char mode;
} Obj0x8006c538;

/* func_8006C454: GPU SPRT packet written at PKT (T8.c465). */
typedef struct {
    unsigned int tag;
    unsigned int code;
    unsigned short x;
    unsigned short y;
    unsigned char u;
    unsigned char v;
    unsigned short clut;
    unsigned short w;
    unsigned short h;
} Sprt0x8006c454;

/* func_8006C2D8, func_8006CB1C: x, y, w, h halfword rectangle (T8.c465). */
typedef struct Rect0x8006c2d8 {
    short x;
    short y;
    short w;
    short h;
} Rect0x8006c2d8;

/* func_800689F4, func_8006DCE0: GPU POLY_F4 packet (T8.c465). */
typedef struct PolyF4_0x800689f4 {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0, x1, y1, x2, y2, x3, y3;
} PolyF4_0x800689f4;

/* Scratchpad +0x2C: 8-byte sort records passed to func_800825E4 (func_8003FE00, T8 w25). */
#define SPAD_ADDR_2C ((void *)SCRATCH(char, 0x2C))

/* Scratchpad word 0x18 (func_8005D930: `lui 0x1f80; lw 0x60`, returned). */
#define SPAD_60 (*SCRATCH(int, 0x60))

/* Fixed RAM addresses above every loaded binary, passed by func_80057A8C to func_80050D80. Kept as constants, not
 * extern symbols: the banked bytes were compiled from constants (a symbol form adds %hi/%lo relocations; untested). */
#define D_80184C60 ((void *)0x80184C60)
#define D_80184D90 ((void *)0x80184D90)
#define D_80184EC0 ((void *)0x80184EC0)
#define D_80184F40 ((void *)0x80184F40)
#define D_80185408 ((void *)0x80185408)
#define D_801857B0 ((void *)0x801857B0)
#define D_801857C0 ((void *)0x801857C0)
#define D_801857C8 ((void *)0x801857C8)
#define D_801857D0 ((void *)0x801857D0)
#define D_801857D8 ((void *)0x801857D8)
#define D_80185858 ((void *)0x80185858)
#define D_801858D8 ((void *)0x801858D8)
#define D_80185958 ((void *)0x80185958)

/* Same kind, args of func_80050D80 from func_800544FC (lui/ori constants). */
#define D_80184A40 ((void *)0x80184A40)
#define D_80184B60 ((void *)0x80184B60)
#define D_80185278 ((void *)0x80185278)
#define D_80185328 ((void *)0x80185328)
#define D_801853C8 ((void *)0x801853C8)
#define D_801853D0 ((void *)0x801853D0)
#define D_801856A0 ((void *)0x801856A0)
#define D_801856B0 ((void *)0x801856B0)

/* Same kind, arg 0 of func_80023990 from func_80022EBC (T8 w24). */
#define D_801850A8 ((void *)0x801850A8)
#define D_80183620 ((void *)0x80183620)

/* Same kind, args of func_80047B8C / func_800484F0 from func_800663EC (T8 w31). */
#define D_80181500 ((void *)0x80181500)
#define D_801819F4 ((void *)0x801819F4)

/* Same kind, arg 1 of func_80048048 from func_8003EB20 (lui/ori constants). */
#define D_80167290 ((void *)0x80167290)
#define D_80178CF4 ((void *)0x80178CF4)
#define D_80179E30 ((void *)0x80179E30)
#define D_8017A040 ((void *)0x8017A040)
#define D_8017A31C ((void *)0x8017A31C)
#define D_8017DAE0 ((void *)0x8017DAE0)

/* Same kind, constant args of the draw helpers in their units (T14.c1: were unit-local #defines). */
#define D_800E0000 ((void *)0x800E0000) /* func_8002F654 */
#define D_80181E28 ((void *)0x80181E28) /* func_8002DF30 */
#define D_801C1500 ((void *)0x801C1500) /* func_8004283C */
#define D_80185020 ((void *)0x80185020) /* func_8002243C */
#define D_80185D50 ((void *)0x80185D50) /* func_80031968 */
#define D_80185D58 ((void *)0x80185D58)
#define D_80185D88 ((void *)0x80185D88)
#define D_801836C8 ((void *)0x801836C8) /* func_8004C960 */
#define D_80185030 ((void *)0x80185030)
#define D_80185038 ((void *)0x80185038)
#define D_80183798 ((void *)0x80183798) /* the func_8004CE64 unit */
#define D_80183928 ((void *)0x80183928)
#define D_80183CD8 ((void *)0x80183CD8)
#define D_80184FE0 ((void *)0x80184FE0)
#define D_80185000 ((void *)0x80185000)
#define D_80185008 ((void *)0x80185008)
#define D_80185010 ((void *)0x80185010)
#define D_80185040 ((void *)0x80185040)
#define D_801850B8 ((void *)0x801850B8)
#define D_801850C8 ((void *)0x801850C8)
#define D_801854C0 ((void *)0x801854C0)
#define D_801854C8 ((void *)0x801854C8)
#define D_801854D0 ((void *)0x801854D0)
#define D_801854D8 ((void *)0x801854D8)
#define D_801854E0 ((void *)0x801854E0)

/* func_800598E0: fixed RAM block header and its first entry (lui/ori constants). */
#define D_8015F500 ((void *)0x8015F500)
#define D_8015F504 ((void *)0x8015F504)

/* func_8001B56C, func_800323EC: one shape for the carved copies (T14.c2). */
typedef struct {
    short x0;
    short x2;
    short x4;
    short x6;
} Rect0x8001b56c;

/* func_8001C478, func_8001C510: one shape for the carved copies (T14.c2). */
typedef struct Ent0x8001c478 {
    char pad0[0xC];
    int flags;
    char pad10[0x8];
} Ent0x8001c478;

/* func_8001CF84, func_8001D028: one shape for the carved copies (T14.c2). */
typedef struct Ent0x8001cf84 {
    unsigned short id;
    int arg;
    int *data;
} Ent0x8001cf84;

/* func_8001D230, func_8001DB64: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x4C];
    unsigned char x4C;
    char pad4d;
    unsigned char x4E;
} Sub0x8001d230;

/* func_8001D338, func_8004A240: one shape for the carved copies (T14.c2). */
typedef struct Pos0x8001d338 {
    short x;
    short y;
    short z;
} Pos0x8001d338;

/* func_8001FB5C, func_8002F080: one shape for the carved copies (T14.c2). */
typedef struct Rec0x8001fb5c {
    int v[8];
} Rec0x8001fb5c;

/* func_80021F78, func_800238E4: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x7C];
    u32 ot;
} OtHolder;

/* func_80021F78, func_8004CE64: one shape for the carved copies (T14.c2). */
typedef struct {
    u32 tag;
    u32 code;
    u16 x;
    u16 y;
    u8 u;
    u8 v;
    u16 clut;
    u16 w;
    u16 h;
} Sprt;

/* func_800223BC, func_80022400: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0xB8];
    short xB8;
    char padba[0xC8 - 0xBA];
    unsigned char xC8;
} Sub22C;

/* func_8002243C, func_80022EBC, func_8002335C, func_80023564, func_80023724, func_80037694, func_8004BC58, func_80050454, func_800514C4, func_80053648, func_80055518, func_800558F4, func_80056A18: one shape for the carved copies (T14.c2). */
typedef struct Box0x8002243c {
    short x, y, w, h;
} Box0x8002243c;

/* func_8002329C, func_80023AFC: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x7C];
    unsigned int ot;
} OtHolder0x8002329c;

/* func_80023CEC, func_80050398: one shape for the carved copies (T14.c2). */
typedef struct {
    unsigned int tag;
    unsigned int code;
    short x;
    short y;
    unsigned short w;
    unsigned short h;
} Tile0x80023cec;

/* func_80023D8C, func_8004B508: one shape for the carved copies (T14.c2). */
typedef struct Unit0x80023d8c {
    char pad0[0x108];
    unsigned short x108;
} Unit0x80023d8c;

/* func_80023ED8, func_8002CC2C: one shape for the carved copies (T14.c2). */
typedef struct { unsigned short x, y, z; unsigned char p, q; } V80x80023ed8;

/* func_80023ED8, func_8002CC2C: one shape for the carved copies (T14.c2). */
typedef struct { unsigned short a, b, c, d; } W80x80023ed8;

/* func_80023ED8, func_8002CC2C: one shape for the carved copies (T14.c2). */
typedef struct { int a, b; } P80x80023ed8;

/* func_80023ED8, func_8002CC2C: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x50];
    int x50;
    char pad54[0x70 - 0x54];
} Shot0x80023ed8;

/* func_80026504, func_8002D154, func_8004BA5C, func_8004C960: one shape for the carved copies (T14.c2). */
typedef struct { unsigned char b[8]; } Name8;

/* func_800266DC, func_8002675C, func_8005FFC0: one shape for the carved copies (T14.c2). */
typedef struct {
    short m[3][3];
    int t[3];
    short x20;
    short x22;
    short x24;
    short pad26;
    short x28;
    short x2A;
    short x2C;
    short pad2E;
    short x30[4];
    short x38[4];
} Cam0x800266dc;

/* func_80026A7C, func_800646B8: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x4A];
    unsigned short x4A;
} Sub0x80026a7c;

/* func_80029458, func_80041810: one shape for the carved copies (T14.c2). */
typedef struct Elem0x80029458 {
    short x0;
    unsigned short x2;
    char pad4[0x18 - 0x4];
} Elem0x80029458;

/* func_8002B874, func_8002BAE8: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad1[0x1];
    unsigned char x1;
    char pad2[0x16];
    unsigned char x18;
} Sub0x8002b874;

/* func_8002C024, func_8002E438: one shape for the carved copies (T14.c2). */
typedef struct Sub0x8002c024 {
    unsigned char x0;
    unsigned char x1;
    char pad2[0x18 - 0x2];
    unsigned char x18;
} Sub0x8002c024;

/* func_8002C83C, func_8002C9B0: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x18];
    unsigned char f18;
} Sub0x8002c83c;

/* func_800317FC, func_8006C2D8: one shape for the carved copies (T14.c2). */
typedef struct Spr0x800317fc {
    unsigned char u;
    unsigned char v;
    unsigned char w;
    unsigned char h;
    signed char x;
    signed char y;
    unsigned char n;
    unsigned char pad;
} Spr0x800317fc;

/* func_8004C960, func_8004DB98, func_8004F710, func_80053648, func_80056A18: one shape for the carved copies (T14.c2). */
typedef struct { unsigned addr : 24; unsigned len : 8; } Tag0x8004c960;

/* func_80053648, func_80056A18: one shape for the carved copies (T14.c2). */
typedef struct Obj0x80053648 {
    char pad0[2];
    unsigned char x2;
    signed char x3;
    char pad4[0x4A];
    unsigned char x4E;
} Obj0x80053648;

/* func_80053DB0, func_8005640C, func_80056C80: one shape for the carved copies (T14.c2). */
typedef struct { char b[8]; } Blk8x0x80053db0;

/* func_8005EEF4, func_800614F0: one shape for the carved copies (T14.c2). */
typedef struct {
    unsigned char *pc;
    char pad04[0x80];
    int *sp;
} Ctx8005EEF4;

/* func_8006335C, func_80063988: one shape for the carved copies (T14.c2). */
typedef struct {
    int pc;
    char pad4[0x84 - 0x4];
    int *src;
} Obj0x8006335c;

/* func_800B27E0: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x40];
    short x40;
    short x42;
    short x44;
    char pad46[0x50 - 0x46];
    int x50;
    char pad54[0x5A - 0x54];
    short x5A;
    char pad5c[0x74 - 0x5C];
    short x74;
    short x76;
    short x78;
    char pad7a[0x85 - 0x7A];
    unsigned char x85;
} S800B27E0;

/* func_80023ED8, func_8002CC2C: one shape for the carved copies (T14.c2). */
typedef struct Rec0x80023ed8 {
    union { int w; struct { unsigned char b0, b1, b2, b3; } b; } u0;
    unsigned short x4;
    unsigned char x6;
    char pad7;
    V80x80023ed8 v8;
    W80x80023ed8 v10;
    int x18;
    unsigned short x1C;
    unsigned char x1E;
    unsigned char x1F;
    P80x80023ed8 x20;
} Rec0x80023ed8;

/* func_80029458, func_80041810: one shape for the carved copies (T14.c2). */
typedef struct Grp0x80029458 {
    char pad0[0xC];
    Elem0x80029458 *xC;
    char pad10[0x18 - 0x10];
} Grp0x80029458;

/* func_8002C83C, func_8002C9B0: one shape for the carved copies (T14.c2). */
typedef struct {
    char pad0[0x40];
    short f40;
    short f42;
    short f44;
    short pad46;
    short f48;
    char pad4a[0x54 - 0x4a];
    unsigned char f54;
    char pad55[0x7c - 0x55];
    unsigned short f7c;
    short pad7e;
    unsigned short f80;
    char pad82[0x8c - 0x82];
    Sub0x8002c83c *f8c;
    char pad90[0x98 - 0x90];
    short f98[8];
} Obj0x8002c83c;

/* func_80023ED8, func_8002CC2C: one shape for the carved copies (T14.c2). */
typedef struct {
    int x0;
    Rec0x80023ed8 *rec;
    signed char x8;
    unsigned char x9;
    signed char xA;
    unsigned char xB;
    unsigned char xC;
    char padD;
    unsigned short xE;
    union { int w; struct { unsigned short a, b; } h; } x10;
} Sub0x80023ed8;

#endif
