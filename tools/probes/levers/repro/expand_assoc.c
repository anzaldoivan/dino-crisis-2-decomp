/* T5.c1 reproducer (our C), expand-cse: association of a mixed add/subtract with a constant. fold (fold-const.c
   associate: :4802) moves the constant to the other operand, so each grouping comes out as the other one in .rtl:
   a: x + 3 - y -> (VAR+CON) - ARG1 -> x - (y - 3) (:4841-4853), the constant rides on y (addu y,-3);
   b: x - (y - 3) -> ARG0 - (VAR-CON) -> (x + 3) - y (:4876-4897), the constant rides on x (addu x,3);
   c: x - y + 3: no split (arg0 has no constant), subu then addu 3. */
int asc_a(int x, int y) { return ((x >> 10) & 3) + 3 - ((y >> 10) & 3); }
int asc_b(int x, int y) { return ((x >> 10) & 3) - (((y >> 10) & 3) - 3); }
int asc_c(int x, int y) { return ((x >> 10) & 3) - ((y >> 10) & 3) + 3; }
