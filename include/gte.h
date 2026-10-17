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
 * CTC2/CFC2), given as a literal.
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
