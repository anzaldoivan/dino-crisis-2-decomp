/* func_8002E920.inc.c -- shared C body of family slus_012_79:0x8002e920 (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   SHARED_F0   exemplar func_80048274
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    int SHARED_F0(Obj8002E920_s8002E920 *o);
    o->x += o->vx;
    o->y += o->vy;
    o->z += o->vz;
    if (SHARED_F0(o) != 0 && !(o->flags & 8)) {
        o->state++;
    }
#undef SHARED_F0
