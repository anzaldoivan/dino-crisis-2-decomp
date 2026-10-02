/* func_800436C8.inc.c -- shared C body of family slus_012_79:0x800436c8 (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   (none: no relocated operand)
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    int dx;
    int dz;

    dx = b->x - a->x;
    dx = (dx < 0) ? -dx : dx;
    dz = b->z - a->z;
    dz = (dz < 0) ? -dz : dz;
    return dx * dx + dz * dz;
