/* T4.c1 reproducer (our C), kit S2: sched1 sinks the def of a single-set pseudo to its use (birthing_insn_p +
   adjust_priority); a pseudo with a 2nd SET is not birthing and keeps its early slot. a: the narrowed param n is
   set once (entry copy folded into the zero_extend), its def sinks to the jal; b: ++n gives n a 2nd SET, the andi and the
   add stay at the top. c/d: the same with a store instead of the call (the game fn shape). */
typedef struct { int f; int x; int y; int z; } S;
extern void g(int);
void birth_a(S *s, int n) { n &= 0xff; s->x = 7; s->y = 9; g(n + 1); }
void birth_b(S *s, int n) { n &= 0xff; s->x = 7; s->y = 9; g(++n); }
void birth_c(S *s, int n) { n &= 0xff; s->x = 7; s->y = 9; s->z = n + 1; }
void birth_d(S *s, int n) { n &= 0xff; s->x = 7; s->y = 9; s->z = ++n; }
