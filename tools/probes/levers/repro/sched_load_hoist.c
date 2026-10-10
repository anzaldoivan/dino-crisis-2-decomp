/* T4.c1 reproducer (our C), load latency 2: the load of a read-modify-write is hoisted above earlier independent
   stores when alias analysis separates them (same base, distinct offsets: a); through a second pointer that may alias
   (b) the load stays behind the stores. c: the RMW written first schedules the same as a. */
typedef struct { int a; short b; char c; int d; } S;
void hoist_a(S *s) { s->b = 0; s->c = 1; s->d |= 2; }
void hoist_b(S *s, S *q) { s->b = 0; s->c = 1; q->d |= 2; }
void hoist_c(S *s) { s->d |= 2; s->b = 0; s->c = 1; }
