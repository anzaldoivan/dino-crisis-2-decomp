/* T4.c1 reproducer (our C), kit S12: two load->store copies. a: fresh pseudos, sched1 batches the loads (lw lw sw sw);
   b: one user var t reused for both, the anti/output dependence on t serialises them (lw sw lw sw). */
typedef struct { int a; int b; int c; int d; } S;
void reuse_a(S *s) { s->c = s->a; s->d = s->b; }
void reuse_b(S *s) { int t; t = s->a; s->c = t; t = s->b; s->d = t; }
