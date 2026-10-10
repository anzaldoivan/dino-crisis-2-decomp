/* T3.c1 reproducer (our C): live-range shrink. a: *p loaded before the call (crosses it); b: loaded after (does not). */
extern void g(int);
int shrink_a(int *p, int q) { int v = *p; g(q); return v + q; }
int shrink_b(int *p, int q) { g(q); return *p + q; }
