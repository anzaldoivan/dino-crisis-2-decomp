/* T4.c2 reproducer (our C): cross-jumping (kit D5; jump2 only, toplev.c:4307). a = both arms end in the same store
 * *r = 5; b = the tails differ (*r = 5 vs *r = 6); c = both arms end in h(1). No shape merged: `.jump2` equals
 * `.sched2` in insn count for a, b, c (in c a `(use (const_int 0))` sits between the else tail and the join label). */
void jump_crossjump_a(int c, int *p, int *q, int *r) { if (c) { *p = 1; *r = 5; } else { *q = 2; *r = 5; } }
void jump_crossjump_b(int c, int *p, int *q, int *r) { if (c) { *p = 1; *r = 5; } else { *q = 2; *r = 6; } }
extern void h(int);
void jump_crossjump_c(int c, int *p, int *q) { if (c) { *p = 1; h(1); } else { *q = 2; h(1); } }
