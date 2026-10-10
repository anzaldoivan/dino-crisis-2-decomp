/* T5.c1 reproducer (our C), expand-cse: cse load reuse vs reload. cse records p->a once and reuses it unless a store
   in between may alias it (cse.c:8040 note_mem_written -> invalidate_memory :8023). a: the store goes to another
   field of the same struct (no alias) -> one lw; b: the store goes through an int * (may alias) -> lw reloaded. */
typedef struct { int a; int b; } S;
int reu_a(S *p) { int t = p->a; p->b = 5; return t + p->a; }
int reu_b(S *p, int *q) { int t = p->a; *q = 5; return t + p->a; }
