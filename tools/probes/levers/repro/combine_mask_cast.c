/* T3.c2 reproducer (our C): mask vs cast of the low half. a = `& 0xffff`, b = `(unsigned short)` cast. */
unsigned int combine_mask_cast_a(unsigned int x) { return (x + 3) & 0xffff; }
unsigned int combine_mask_cast_b(unsigned int x) { return (unsigned short)(x + 3); }
int combine_mask_cast_c(int x) { return (short)(x + 3); }
int combine_mask_cast_d(int x) { return ((x + 3) << 16) >> 16; }
