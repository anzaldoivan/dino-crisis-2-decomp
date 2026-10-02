/* T3.c2 reproducer (our C): compare respelling. a = `x < 8`, b = `x <= 7`, c = `!(x >= 8)` on unsigned. */
int combine_compare_a(unsigned int x) { return x < 8; }
int combine_compare_b(int x) { return (x & 0xffff) < 8; }
int combine_compare_c(unsigned int x) { return (x >> 3) == 0; }
