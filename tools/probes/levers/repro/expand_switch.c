/* T5.c1 reproducer (our C), expand-cse: switch lowering. expand_end_case (stmt.c:5001) emits a tablejump when the
   case count reaches CASE_VALUES_THRESHOLD (5 without casesi, stmt.c:5127), else a compare tree (emit_case_nodes,
   stmt.c:5823). a: 4 dense cases -> compare tree; b: 5 dense cases -> jump table. */
int sw_a(int x) { switch (x) { case 0: return 7; case 1: return 3; case 2: return 9; case 3: return 4; } return 0; }
int sw_b(int x) { switch (x) { case 0: return 7; case 1: return 3; case 2: return 9; case 3: return 4; case 4: return 8; } return 0; }
