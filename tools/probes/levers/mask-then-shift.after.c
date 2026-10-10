/* lever mask-then-shift (T3.c2, group combine), after: byte field spelled mask-then-shift `(x & 0xff00) >> 8`.
 * Game fn slus_012_79:0x8001B538. Our C; self-contained (tools/probes/levers/README.md). */
extern unsigned char D_800ABF50[];

void func_8001B538(unsigned int x) {
    unsigned int g = (x & 0xff00) >> 8, b = (x >> 16) & 0xff;
    D_800ABF50[189] = x; D_800ABF50[45] = x;
    D_800ABF50[190] = g; D_800ABF50[46] = g;
    D_800ABF50[191] = b; D_800ABF50[47] = b;
}
