/* func_8002D78C.inc.c -- shared C body of family slus_012_79:0x8002d78c (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   SHARED_D0   exemplar D_80088E10
 *   SHARED_D1   exemplar D_80089060
 *   SHARED_F0   exemplar func_8002B614
 *   SHARED_F1   exemplar func_8002B41C
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    extern void (*SHARED_D0[])(Obj_s8002D78C *);
    extern void (*SHARED_D1[])(Obj_s8002D78C *);
    void SHARED_F0(Obj_s8002D78C *);
    void SHARED_F1(Obj_s8002D78C *);
    SHARED_D0[p->state](p);
    SHARED_D1[p->mode](p);
    SHARED_F0(p);
    if (--p->timer == 0) {
        SHARED_F1(p);
    }
#undef SHARED_D0
#undef SHARED_D1
#undef SHARED_F0
#undef SHARED_F1
