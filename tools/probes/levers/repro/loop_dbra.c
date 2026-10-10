/* T5.c2 reproducer (our C): check_dbra_loop (loop.c:7654) reverses a count-up loop whose biv is used only by the
   exit test into a count-down ("Reversed loop", loop.c:8162). a: i counts 0..n, unused in the body: reversed;
   b: i is stored in the body: not reversible, stays count-up; c: the count-down form written directly. */
extern void g(void);
void dbra_a(int n) { int i; for (i = 0; i < n; i++) g(); }
void dbra_b(int *p, int n) { int i; for (i = 0; i < n; i++) { *p = i; g(); } }
void dbra_c(int n) { while (n-- > 0) g(); }
