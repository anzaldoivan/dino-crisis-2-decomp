/* include_asm.h — INCLUDE_ASM / INCLUDE_RODATA for the pinned triple (docs/ops/decomp-environment.md).
 * Top-level __asm__ that maspsx passes through to as: .include of "<dir>/<name>.s" inside .text, bracketed by
 * .set noreorder/noat (the splat output is scheduled by hand). -G0 build: no __maspsx_include_asm_hack wrapper needed. */
#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

#define INCLUDE_ASM(FOLDER, NAME)                                                                                  \
    __asm__(".pushsection .text\n"                                                                                 \
            "\t.align\t2\n"                                                                                        \
            "\t.set noreorder\n"                                                                                   \
            "\t.set noat\n"                                                                                        \
            ".include \"" FOLDER "/" #NAME ".s\"\n"                                                                \
            "\t.set reorder\n"                                                                                     \
            "\t.set at\n"                                                                                          \
            ".popsection")

#define INCLUDE_RODATA(FOLDER, NAME)                                                                               \
    __asm__(".pushsection .rodata\n"                                                                               \
            ".include \"" FOLDER "/" #NAME ".s\"\n"                                                                \
            ".popsection")

#endif
