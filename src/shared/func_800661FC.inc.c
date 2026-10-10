/* func_800661FC.inc.c -- shared C body of family slus_012_79:0x800661fc (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   (none: no relocated operand)
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    Hdr_s800661FC *h = obj->hdr;
    A28_s800661FC *a = h->a;
    A34_s800661FC *b = h->b;
    unsigned int i;

    for (i = 0; i < h->na; i++, a++) {
        a->b2 = c;
        a->g2 = c;
        a->r2 = c;
        a->b1 = c;
        a->g1 = c;
        a->r1 = c;
        a->b0 = c;
        a->g0 = c;
        a->r0 = c;
    }
    for (i = 0; i < h->nb; i++, b++) {
        b->b3 = c;
        b->g3 = c;
        b->r3 = c;
        b->b2 = c;
        b->g2 = c;
        b->r2 = c;
        b->b1 = c;
        b->g1 = c;
        b->r1 = c;
        b->b0 = c;
        b->g0 = c;
        b->r0 = c;
    }
