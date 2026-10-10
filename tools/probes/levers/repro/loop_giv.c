/* T5.c2 reproducer (our C): strength_reduce (loop.c:3690) turns p[i] (i*4 + p) into a pointer giv ("giv at %d reduced
   to"). a: variable bound, i only an index: pointer giv, then check_dbra_loop (loop.c:7654) reverses i to count down to
   0 (blez guard, bne); b: constant bound 16: "Reversed loop and added reg_nonneg": the whole walk runs backwards (p+60
   down, counter 15 down, bgez) and the duplicate pointer biv goes ("Reg %d: biv eliminated", loop.c:5137);
   c: i also stored in the body: not reversible, i stays a count-up biv (slt 16) beside the pointer giv. */
void giv_a(int *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0; }
void giv_b(int *p) { int i; for (i = 0; i < 16; i++) p[i] = 0; }
void giv_c(int *p, int *q) { int i; for (i = 0; i < 16; i++) { p[i] = 0; *q = i; } }
