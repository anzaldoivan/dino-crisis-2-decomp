/* func_800D5C40.inc.c -- shared C body of family psx_bin_st6:0x800d5c40 (exact class, config/families.tsv; T4,
 * Phase 1.6). Included inside a unit's function braces after its signature line, never compiled on its own
 * (not a unit: tools/compile_only.sh and the harness skip *.inc.c). The unit binds, by one-line #define:
 *   SHARED_D0  Entry table   (exemplar D_800D6A70)
 *   SHARED_F0  callee         (exemplar func_80047814)
 * tools/propagate.py derives a member's bindings from its relocated operands and writes the instantiation. */
    extern Entry SHARED_D0[];
    extern void SHARED_F0(Obj *p);
    int i;

    SHARED_D0[0].c = p->x98;
    for (i = 0; i < 8; i++) {
        SHARED_D0[i + 1].c = p->x9e[i] + SHARED_D0[i].c;
    }
    SHARED_F0(p);
#undef SHARED_D0
#undef SHARED_F0
