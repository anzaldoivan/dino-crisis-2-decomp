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
    char pad0[0x50];
    int flags;                  /* lw 0x50(a2); andi 1 */
    char pad54[0x64 - 0x54];
    int id;                     /* psx_bin_st1 func_800D9000: lw 0x64 of WORK->units, passed as arg 0 */
    char pad68[0xFD - 0x68];
    unsigned char side;         /* lbu 0xFD(a2); sb 0xFD(t0) */
    char padfe[0x260 - 0xFE];
} Unit;

/* Scratchpad-resident work block (word 0 of the scratchpad). Total size unproven. Field users in comments. */
typedef struct {
    char pad0[0x228];
    int x228;                   /* func_80052634: lw v1,0(0x1F800000); lw v0,0x228(v1); addiu; sw v0,0x228(v1) */
    char pad22c[0x318 - 0x22C];
    char x318[0xCE];            /* func_80036E94: address passed to func_80037C9C */
    unsigned char x3E6;         /* func_80036E94: lbu, compared with x3E7 */
    unsigned char x3E7;         /* func_80036E94: lbu, sb from x3E6 */
    char pad3e8[0x4E0 - 0x3E8];
    Unit *units;                /* lw 0x4E0(v0); units[0] is the player (func_8003D2D4, func_8003D58C, st1) */
    char pad4e4[0x5EC - 0x4E4];
    AreaTbl *areas;             /* func_8003D2D4, func_8003D58C: lw 0x5EC */
    char pad5f0[0x5FC - 0x5F0];
    Elem28 *x5FC;               /* func_80063CC4: sw of D_800B4A68 */
    char pad600[0x6E8 - 0x600];
    char x6E8;                  /* func_8004B7B8: sb */
    char pad6e9[0xC2B - 0x6E9];
    unsigned char xC2B;         /* func_80039654: lbu, compared with 2 */
    unsigned char xC2C;         /* func_8004587C: lbu, arg 1 of func_800458F0 */
    unsigned char xC2D;         /* func_8004587C: lbu, arg 0 of func_800458F0 */
    char padc2e[0xE64 - 0xC2E];
    int xE64;                   /* func_8002219C: lw, decremented, sw */
    int xE68;                   /* func_8002219C: lw */
} Work;

/* Scratchpad word 0: `lui 0x1f80; lw 0(v0)`. The only home of this address literal. */
#define WORK (*(Work **)0x1F800000)

/* Scratchpad word 1: primitive packet cursor (func_8002204C: lw, stores through it, advances by 12). */
#define PKT (*(u32 **)0x1F800004)

/* Scratchpad word 0x18 (func_8005D930: `lui 0x1f80; lw 0x60`, returned). */
#define SPAD_60 (*(int *)0x1F800060)

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

#endif
