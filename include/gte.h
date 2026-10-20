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
 * CPU load a macro needs is C, its value passed in as an operand (GTE_CTC2_024, T28).
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

/* commands: two nops, then cop2 <encoding> */
#define GTE_CMD(enc) __asm__ volatile("nop\n\tnop\n\tcop2 %0" : : "i"(enc))
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
