/* T4.c2 reproducer (our C): fill_simple_delay_slots (kit D1/D2). a = an independent store before the call (backward
 * fill of the jal slot); b = the store after the call (the copy of x into a callee-saved reg fills the slot instead);
 * c = argument setup before the call (the `x + 1` into $a0 fills the slot); d = back-to-back calls: nothing
 * fillable, cc1 prints the jal outside `.set noreorder` and maspsx adds the nop (kit D2). */
extern void g(void); extern void h(int);
void reorg_fill_simple_a(int *p, int x) { *p = x; g(); p[1] = 0; }
void reorg_fill_simple_b(int *p, int x) { g(); *p = x; p[1] = 0; }
void reorg_fill_simple_c(int x) { h(x + 1); }
void reorg_fill_simple_d(void) { g(); g(); }
