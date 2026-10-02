/* T3.c1 reproducer (our C): two call-crossing values, first-set order swapped. a: x set first; b: y set first. */
extern int g(int);
extern void h(int, int);
void decl_a(void) { int x, y; x = g(1); y = g(2); g(3); h(x, y); }
void decl_b(void) { int x, y; y = g(2); x = g(1); g(3); h(x, y); }
