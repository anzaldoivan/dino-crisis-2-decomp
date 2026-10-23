/* gte.h — GTE (cop2) access for C drafts and units (T10.c10, GTE lane). Our own text; source = the public MIPS
 * coprocessor-2 mnemonics (mtc2/mfc2/ctc2/cfc2/lwc2/swc2, `cop2 <imm25>`) and the publicly documented GTE command
 * encodings (psx-spx "GTE Command Encoding"). No vendor SDK file or macro name is used.
 *
 * The pinned assembler (binutils mipsel `as`) rejects the GTE command mnemonics (rtps, mvmva, ...), so each command
 * macro emits `cop2 <imm25>` with the public encoding: funct in bits 0-5, lm bit 10, cv bits 13-14, v bits 15-16,
 * mx bits 17-18, sf bit 19, plus the documented fixed high bits of each command.
 *
 * This is the only file where `__asm__` is allowed in C (tools/banked.py, tools/wave.py VERBATIM): a draft or unit
 * that #defines one of the macro names below itself is refused. Each command macro carries the two nops the GTE
 * needs after a data move (part of the command, as compiled code shows it). gcc allocates the CPU registers through
 * the operand constraints; `r` is a GTE register number 0-31 (data regs for MTC2/MFC2/LWC2/SWC2, control regs for
 * CTC2/CFC2), given as a literal. Every template holds cop2 instructions and nops only (the floor's `gte` kind); a
 * CPU load a macro needs is C, its value passed in as an operand (GTE_CTC2_024, T28). Exception: the base-register
 * forms GTE_LD_V3/GTE_ST_V3 (GTE_LD_COL/GTE_ST_COL), GTE_CTC2_ROT and GTE_CTC2_024_R (T10.1) keep their CPU moves
 * in the template, where the match needs it (C loads fold the base's `addiu` into the offset).
 */
#ifndef GTE_H
#define GTE_H

/* data and control moves */
#define GTE_MTC2(r, val) __asm__ volatile("mtc2 %0, $" #r : : "r"(val))
#define GTE_MFC2(r, var) __asm__ volatile("mfc2 %0, $" #r : "=r"(var))
#define GTE_CTC2(r, val) __asm__ volatile("ctc2 %0, $" #r : : "r"(val))
#define GTE_CFC2(r, var) __asm__ volatile("cfc2 %0, $" #r : "=r"(var))
#define GTE_LWC2(r, mem) __asm__ volatile("lwc2 $" #r ", %0" : : "m"(mem))
#define GTE_SWC2(r, mem) __asm__ volatile("swc2 $" #r ", %0" : "=m"(mem))
/* swc2 through a base register: `swc2 $r, off(base)`; base is an "r" operand, off a literal byte offset */
#define GTE_SWC2_AT(r, off, base) __asm__ volatile("swc2 $" #r ", " #off "(%0)" : : "r"(base) : "memory")
/* three words at ptr (offsets 0, 4, 8) into control regs 0, 2, 4 through fixed $12-$14 (lw/ctc2 interleaved):
 * the loads are C into register variables, each ctc2 its own asm, so the lw/ctc2 order stays. The words are read
 * as fields of gte_w3_ (no offset cast); the three register variables are pins at the call site (T28), each
 * covered by the block-comment `!FAKE:` marker on its own line here (floor 0.6.3: a call site's own markers never
 * cover a macro's pins). Block comments only inside the #define: a `//` would swallow the continued line. */
struct gte_w3_ {
    int w0, w1, w2;
};
#define GTE_CTC2_024(ptr)                                                          \
    do {                                                                           \
        register int gte_w0_ asm("$12") = ((volatile struct gte_w3_ *)(ptr))->w0; /* !FAKE: T28, unpinned "r" operands fail sha1 (gcc picks other regs; original uses $12-$14) */ \
        register int gte_w1_ asm("$13") = ((volatile struct gte_w3_ *)(ptr))->w1; /* !FAKE: T28, unpinned "r" operands fail sha1 (gcc picks other regs; original uses $12-$14) */ \
        __asm__ volatile("ctc2 %0, $0" : : "r"(gte_w0_));                          \
        {                                                                          \
            register int gte_w2_ asm("$14") = ((volatile struct gte_w3_ *)(ptr))->w2; /* !FAKE: T28, unpinned "r" operands fail sha1 (gcc picks other regs; original uses $12-$14) */ \
            __asm__ volatile("ctc2 %0, $2" : : "r"(gte_w1_));                      \
            __asm__ volatile("ctc2 %0, $4" : : "r"(gte_w2_));                      \
        }                                                                          \
    } while (0)
/* three words at ptr (offsets 0, 4, 8) into control regs 0, 2, 4 through fixed $8-$10 (T10.1): same shape as
 * GTE_CTC2_024, other pins (three lw, then three ctc2). */
#define GTE_CTC2_024_T(ptr)                                                        \
    do {                                                                           \
        register int gte_w0_ asm("$8") = ((volatile struct gte_w3_ *)(ptr))->w0; /* !FAKE: T10.1, unpinned "r" operands fail sha1 (original uses $8-$10) */ \
        register int gte_w1_ asm("$9") = ((volatile struct gte_w3_ *)(ptr))->w1; /* !FAKE: T10.1, unpinned "r" operands fail sha1 (original uses $8-$10) */ \
        register int gte_w2_ asm("$10") = ((volatile struct gte_w3_ *)(ptr))->w2; /* !FAKE: T10.1, unpinned "r" operands fail sha1 (original uses $8-$10) */ \
        __asm__ volatile("ctc2 %0, $0" : : "r"(gte_w0_));                          \
        __asm__ volatile("ctc2 %0, $2" : : "r"(gte_w1_));                          \
        __asm__ volatile("ctc2 %0, $4" : : "r"(gte_w2_));                          \
    } while (0)
/* lwc2 through a base register: `lwc2 $r, off(base)` (T10.1.c13), the load twin of GTE_SWC2_AT */
#define GTE_LWC2_AT(r, off, base) __asm__ volatile("lwc2 $" #r ", " #off "(%0)" : : "r"(base) : "memory")
/* an SVECTOR at base into data regs 0, 1 in one statement: `lwc2 $0, 0(base); lwc2 $1, 4(base)` (T10.1.c13) */
#define GTE_LWC2_01(base) __asm__ volatile("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" : : "r"(base) : "memory")
/* three words at base into IR3, IR1, IR2 in one statement: `lwc2 $11, 8(base); lwc2 $9, 0(base); lwc2 $10, 4(base)`
 * (T10.1.c23, that order) */
#define GTE_LD_IR(base) __asm__ volatile("lwc2 $11, 8(%0)\n\tlwc2 $9, 0(%0)\n\tlwc2 $10, 4(%0)" : : "r"(base) : "memory")
/* MAC1-3 to three words at base in one statement: `swc2 $25, 0(base); swc2 $26, 4(base); swc2 $27, 8(base)` (T10.1.c23) */
#define GTE_ST_MAC(base) __asm__ volatile("swc2 $25, 0(%0)\n\tswc2 $26, 4(%0)\n\tswc2 $27, 8(%0)" : : "r"(base) : "memory")
/* five words at base (offsets 0-16, a rotation matrix) into control regs 0-4 through fixed $12-$14 (T10.1.c13):
 * lw $12,$13 / ctc2 0,1 / lw $12,$13,$14 / ctc2 2,3,4. The loads stay in the template: the original passes the
 * matrix in a register (`addiu rX, s, off` then offsets 0-16); C loads fold the first one into s+off. Mixed. */
#define GTE_CTC2_ROT(base)                                                         \
    __asm__ volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $0\n\tctc2 $13, $1\n\t" \
                     "lw $12, 8(%0)\n\tlw $13, 12(%0)\n\tlw $14, 16(%0)\n\t"       \
                     "ctc2 $12, $2\n\tctc2 $13, $3\n\tctc2 $14, $4"                \
                     : : "r"(base) : "$12", "$13", "$14")
/* GTE_CTC2_024 with the loads in the template, through a base register (T10.1.c13): lw $12,$13 / ctc2 0 /
 * lw $14 / ctc2 2,4. Mixed, for the same reason as GTE_CTC2_ROT. */
#define GTE_CTC2_024_R(base)                                                       \
    __asm__ volatile("lw $12, 0(%0)\n\tlw $13, 4(%0)\n\tctc2 $12, $0\n\t"          \
                     "lw $14, 8(%0)\n\tctc2 $13, $2\n\tctc2 $14, $4"               \
                     : : "r"(base) : "$12", "$13", "$14")
/* SVECTOR (halfwords at base+0, +2, +4) to data regs 9-11 and back, through fixed $12-$14 (T10.1.c13). C halfword
 * moves into pins (fields of gte_h3_, no offset cast), one mtc2/mfc2 asm each: the floor's `gte` kind, where the
 * base is a live register (no fold); GTE_LD_V3/GTE_ST_V3 below where it is not. */
struct gte_h3_ {
    unsigned short h0, h1, h2;
};
#define GTE_LD_SV(base)                                                            \
    do {                                                                           \
        register int gte_h0_ asm("$12") = ((volatile struct gte_h3_ *)(base))->h0; /* !FAKE: T10.1.c13, the original moves the vector through $12-$14 */ \
        register int gte_h1_ asm("$13") = ((volatile struct gte_h3_ *)(base))->h1; /* !FAKE: T10.1.c13, the original moves the vector through $12-$14 */ \
        register int gte_h2_ asm("$14") = ((volatile struct gte_h3_ *)(base))->h2; /* !FAKE: T10.1.c13, the original moves the vector through $12-$14 */ \
        __asm__ volatile("mtc2 %0, $9" : : "r"(gte_h0_));                          \
        __asm__ volatile("mtc2 %0, $10" : : "r"(gte_h1_));                         \
        __asm__ volatile("mtc2 %0, $11" : : "r"(gte_h2_));                         \
    } while (0)
#define GTE_ST_SV(base)                                                            \
    do {                                                                           \
        register int gte_h0_ asm("$12"); /* !FAKE: T10.1.c13, the original moves the vector through $12-$14 */ \
        register int gte_h1_ asm("$13"); /* !FAKE: T10.1.c13, the original moves the vector through $12-$14 */ \
        register int gte_h2_ asm("$14"); /* !FAKE: T10.1.c13, the original moves the vector through $12-$14 */ \
        __asm__ volatile("mfc2 %0, $9" : "=r"(gte_h0_));                           \
        __asm__ volatile("mfc2 %0, $10" : "=r"(gte_h1_));                          \
        __asm__ volatile("mfc2 %0, $11" : "=r"(gte_h2_));                          \
        ((volatile struct gte_h3_ *)(base))->h0 = gte_h0_;                            \
        ((volatile struct gte_h3_ *)(base))->h1 = gte_h1_;                            \
        ((volatile struct gte_h3_ *)(base))->h2 = gte_h2_;                            \
    } while (0)
/* save control regs 0, 2, 4 into fixed $13-$15 and restore them (T10.1). GTE_SAVE_024 declares the three pinned
 * register variables, so it opens the block (declarations first); GTE_RESTORE_024 in the same block reads them. */
#define GTE_SAVE_024()                                                             \
    register int gte_c0_ asm("$13"); /* !FAKE: T10.1, the saved ctrl regs live in $13-$15 in the original */ \
    register int gte_c2_ asm("$14"); /* !FAKE: T10.1, the saved ctrl regs live in $13-$15 in the original */ \
    register int gte_c4_ asm("$15"); /* !FAKE: T10.1, the saved ctrl regs live in $13-$15 in the original */ \
    __asm__ volatile("cfc2 %0, $0" : "=r"(gte_c0_));                               \
    __asm__ volatile("cfc2 %0, $2" : "=r"(gte_c2_));                               \
    __asm__ volatile("cfc2 %0, $4" : "=r"(gte_c4_))
#define GTE_RESTORE_024()                                                          \
    do {                                                                           \
        __asm__ volatile("ctc2 %0, $0" : : "r"(gte_c0_));                          \
        __asm__ volatile("ctc2 %0, $2" : : "r"(gte_c2_));                          \
        __asm__ volatile("ctc2 %0, $4" : : "r"(gte_c4_));                          \
    } while (0)
/* three halfwords at base+0, +s, +2s (s = 2: an SVECTOR; s = 6: a MATRIX column) to data regs 9-11 and back,
 * through fixed $12-$14 (T10.1; stride form T10.1.c13). The halfword moves stay inside the template: the original
 * passes the base in a register (`addiu rX, base, off` then 0/s/2s), which C loads would fold into the offset.
 * Mixed templates (the floor's `asm` kind); GTE_LD_SV/GTE_ST_SV are the C-load forms where no fold happens. */
#define GTE_LD_V3(base, s)                                                         \
    __asm__ volatile("lhu $12, 0(%0)\n\tlhu $13, %1(%0)\n\tlhu $14, %2(%0)\n\t"    \
                     "mtc2 $12, $9\n\tmtc2 $13, $10\n\tmtc2 $14, $11"              \
                     : : "r"(base), "i"(s), "i"(2 * (s)) : "$12", "$13", "$14")
#define GTE_ST_V3(base, s)                                                         \
    __asm__ volatile("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\t"          \
                     "sh $12, 0(%0)\n\tsh $13, %1(%0)\n\tsh $14, %2(%0)"           \
                     : : "r"(base), "i"(s), "i"(2 * (s)) : "$12", "$13", "$14", "memory")
#define GTE_LD_COL(base) GTE_LD_V3(base, 6)
#define GTE_ST_COL(base) GTE_ST_V3(base, 6)

/* commands: two nops, then cop2 <encoding> */
#define GTE_CMD(enc) __asm__ volatile("nop\n\tnop\n\tcop2 %0" : : "i"(enc))
/* one leading nop (T10.1: a command right after lwc2, as compiled code shows it) */
#define GTE_CMD1(enc) __asm__ volatile("nop\n\tcop2 %0" : : "i"(enc))
#define GTE_OP1(sf) GTE_CMD1(0x170000C | ((sf) << 19))
#define GTE_RTPS() GTE_CMD(0x0180001)
#define GTE_RTPT() GTE_CMD(0x0280030)
#define GTE_NCLIP() GTE_CMD(0x1400006)
#define GTE_OP(sf) GTE_CMD(0x170000C | ((sf) << 19))
#define GTE_NCCS() GTE_CMD(0x108041B)
#define GTE_NCCT() GTE_CMD(0x118043F)
#define GTE_GPF(sf) GTE_CMD(0x190003D | ((sf) << 19))
#define GTE_GPL(sf) GTE_CMD(0x1A0003E | ((sf) << 19))
#define GTE_MVMVA(sf, mx, v, cv, lm) \
    GTE_CMD(0x0400012 | ((sf) << 19) | ((mx) << 17) | ((v) << 15) | ((cv) << 13) | ((lm) << 10))

#endif
