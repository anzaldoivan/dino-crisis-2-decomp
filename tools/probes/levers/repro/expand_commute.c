/* T5.c1 reproducer (our C), expand-cse: operand order of the addu in a scaled-index address. expand_expr PLUS_EXPR
   (expr.c:7035 "Put a constant term last and put a multiplication first") swaps a MULT second operand to the front.
   a: b + i * 4 (pointer_int_sum keeps the MULT_EXPR) -> (plus (mult i 4) b), index first; b: b + (i << 2) is an
   LSHIFT_EXPR, no swap -> (plus b (ashift i 2)), base first. c: array indexing p[i] scales by a MULT too (= a). */
int com_a(char *b, int i) { return *(int *)(b + i * 4 + 8); }
int com_b(char *b, int i) { return *(int *)(b + (i << 2) + 8); }
int com_c(int *p, int i) { return p[i + 2]; }
