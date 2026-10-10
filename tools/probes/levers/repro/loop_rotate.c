/* T5.c2 reproducer (our C): exit-test rotation. .rtl: the while test sits after NOTE_INSN_LOOP_BEG (c-parse.y:1750
   expand_start_loop); .jump: duplicate_loop_exit_test (jump.c:2551) copies it before LOOP_BEG as a guard and marks the
   bottom test with NOTE_INSN_LOOP_VTOP (jump.c:2766). a: while: guard + bottom test; b: do-while (continue_elsewhere,
   c-parse.y:1656): no guard, no VTOP; c: for(;;) + if-break at the top: rotated like a (guard + VTOP). */
extern int g(int);
void rot_a(int n) { while (n > 0) n = g(n); }
void rot_b(int n) { do n = g(n); while (n > 0); }
void rot_c(int n) { for (;;) { if (n <= 0) break; n = g(n); } }
