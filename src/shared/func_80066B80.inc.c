/* func_80066B80.inc.c -- shared C body of family slus_012_79:0x80066b80 (exact class, config/families.tsv; T7.c27
 * propagate.py --register). Included inside a unit's function braces after its signature line, never
 * compiled on its own (*.inc.c is not a unit). The unit binds, by one-line #define:
 *   SHARED_F0   exemplar func_8004A2C4
 * tools/propagate.py derives a member's bindings from its relocated operands. */
    extern int SHARED_F0(int a, int b);
    int d;

    d = (SHARED_F0(a, b) - ang) & 0xFFF;
    if (d < 0x800 && d < step * 2) {
        step = d;
    } else if (d >= 0x800 && 0x1000 - d < step * 2) {
        step = 0x1000 - d;
    }
    if (d <= 0x800) {
        return step;
    }
    return -step;
#undef SHARED_F0
