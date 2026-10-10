/* T4.c2 reproducer (our C): fill_eager_delay_slots / fill_slots_from_thread (kit D3/D4).
 * a = both threads start with a load: may_trap_p (reorg.c:2612) bars the steal, the bne is printed outside `.set noreorder` (unfilled); b = an independent store
 * before the test fills the slot (fill_simple, D1); c = a scan loop (both branches filled from their threads; D4
 * copy-forward not isolated); d = non-trapping thread heads (li): the slot is stolen from a thread (D3). */
extern int g(int);
int reorg_eager_a(int *p) { if (*p != 0) return p[1] + 3; return p[2] + 5; }
int reorg_eager_b(int *p, int *q) { int t = *p; *q = 7; if (t != 0) return p[1] + 3; return p[2] + 5; }
int reorg_eager_c(int *p, int n) { int i = 0; while (p[i] != n) i = i + 1; return i; }
int reorg_eager_d(int *p) { if (*p != 0) return 3; return 5; }
