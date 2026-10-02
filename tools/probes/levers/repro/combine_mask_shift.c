/* T3.c2 reproducer (our C): byte field extraction. a = mask-then-shift (2 insns), b = shift-then-mask,
 * c = mask-then-shift with a constant that needs its own load (3 insns: combine rewrites it to shift-then-mask). */
unsigned int combine_mask_shift_a(unsigned int x) { return (x & 0xff00) >> 8; }
unsigned int combine_mask_shift_b(unsigned int x) { return (x >> 8) & 0xff; }
unsigned int combine_mask_shift_c(unsigned int x) { return (x & 0xff0000) >> 16; }
