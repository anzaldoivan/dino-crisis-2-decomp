/* func_8002E604.inc.c -- shared C body of family slus_012_79:0x8002e604 (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   SHARED_D0   exemplar D_800891D8
 *   SHARED_F0   exemplar func_8002E760
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    extern void (*SHARED_D0[])(Obj8002E604_s8002E604 *);
    extern void SHARED_F0(Obj8002E604_s8002E604 *);
    SHARED_D0[obj->state](obj);
    SHARED_F0(obj);
#undef SHARED_D0
#undef SHARED_F0
