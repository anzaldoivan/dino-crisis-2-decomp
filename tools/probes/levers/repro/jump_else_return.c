/* T4.c2 reproducer (our C): if/else with a returning then-arm. a = `if (x == 0) { A; return 1; } B; return 0;`
 * b = the same with the test inverted and the arms swapped; c = if/else joining (no return in the arms). */
typedef struct { int v; int *ptr; char f8; signed char f9; } S;
int jump_else_return_a(S *s) { if (s->f9 == 0) { s->f8 = 0; return 1; } s->f9--; s->v = *s->ptr++; return 0; }
int jump_else_return_b(S *s) { if (s->f9 != 0) { s->f9--; s->v = *s->ptr++; return 0; } s->f8 = 0; return 1; }
void jump_else_return_c(S *s) { if (s->f9 == 0) s->f8 = 0; else { s->f9--; s->v = *s->ptr++; } s->v++; }
