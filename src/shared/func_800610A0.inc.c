/* func_800610A0.inc.c -- shared C body of family slus_012_79:0x800610a0 (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   SHARED_F0   exemplar func_80034D88
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    extern void SHARED_F0(int a, int b, int c);
    SHARED_F0(ctx->arg[2], ctx->arg[1], ctx->arg[0]);
    ctx->pc += 2;
    return 0;
#undef SHARED_F0
