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

/* slus_012_79 func_80037E18: size 0x260 proven by `addiu a2,a2,608` and the n*19*32 bound. */
typedef struct {
    char pad0[0x50];
    int flags;                  /* lw 0x50(a2); andi 1 */
    char pad54[0xFD - 0x54];
    unsigned char side;         /* lbu 0xFD(a2); sb 0xFD(t0) */
    char padfe[0x260 - 0xFE];
} Unit;

/* slus_012_79 func_80037E18: scratchpad-resident work block. Total size unproven. */
typedef struct {
    char pad0[0x228];
    int x228;                   /* func_80052634: lw v1,0(0x1F800000); lw v0,0x228(v1); addiu; sw v0,0x228(v1) */
    char pad22c[0x4E0 - 0x22C];
    Unit *units;                /* lw 0x4E0(v0) */
} Work;

/* Scratchpad word 0: `lui 0x1f80; lw 0(v0)`. The only home of this address literal. */
#define WORK (*(Work **)0x1F800000)

#endif
