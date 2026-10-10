/* T4.c1 reproducer (our C), kit S1: independent stores of equal priority keep source (LUID) order through sched1 and
   sched2 (rank_for_schedule's last rule, lower LUID first). a and b differ only by the order of the b/c statements. */
typedef struct { int a; short b; char c; } S;
void luid_a(S *s) { s->b = 0; s->c = 1; s->a = 2; }
void luid_b(S *s) { s->c = 1; s->b = 0; s->a = 2; }
