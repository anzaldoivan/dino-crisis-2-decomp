/* func_800264AC.inc.c -- shared C body of family slus_012_79:0x800264ac (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   SHARED_D0   exemplar D_80087CEC
 *   SHARED_F0   exemplar func_8002BDE4
 *   SHARED_F1   exemplar func_80026504
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    extern u8 SHARED_D0[];
    extern u8 SHARED_F0(int);
    extern void SHARED_F1(void *, void *);
    Sub_s800264AC *sub = (Sub_s800264AC *)(obj + 0xC);
    u8 v;

    v = SHARED_F0(0);
    sub->state = v;
    if ((u8)(v - 1) < 2) {
        SHARED_F1(obj, SHARED_D0);
    }
#undef SHARED_D0
#undef SHARED_F0
#undef SHARED_F1
