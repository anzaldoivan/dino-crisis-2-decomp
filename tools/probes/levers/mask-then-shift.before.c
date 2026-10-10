/* lever mask-then-shift (T3.c2, group combine), before: byte field spelled shift-then-mask `(x >> 8) & 0xff`.
 * Game fn slus_012_79:0x8001B538. Our C; self-contained (tools/probes/levers/README.md). */
extern unsigned char D_800ABF50[];

void func_8001B538(unsigned int x) {
    unsigned int g = (x >> 8) & 0xff, b = (x >> 16) & 0xff;
    D_800ABF50[189] = x; D_800ABF50[45] = x;
    D_800ABF50[190] = g; D_800ABF50[46] = g;
    D_800ABF50[191] = b; D_800ABF50[47] = b;
}
