/* T5.c2 reproducer (our C): scan_loop/move_movables (loop.c:634, :1739) hoist a loop-invariant set out of the loop
   (" moved to %d", loop.c:1876). a: while (n--) compares n against -1: the constant load is hoisted; b: the same walk
   as a counted for: reversed to count down to 0 (no constant, nothing moved); c: invariant load s->x with no store in
   the loop: hoisted; d: a store through int * may alias s->x (no strict aliasing in 2.95.2 by default): not hoisted. */
typedef struct N { int x; struct N *next; } N;
N *hoist_a(N *p, int n) { while (n--) p = p->next; return p; }
N *hoist_b(N *p, int n) { int i; for (i = 0; i < n; i++) p = p->next; return p; }
int hoist_c(N *s, int n) { int i, t = 0; for (i = 0; i < n; i++) t += s->x + i; return t; }
void hoist_d(N *s, int *p, int n) { int i; for (i = 0; i < n; i++) p[i] = s->x + i; }
