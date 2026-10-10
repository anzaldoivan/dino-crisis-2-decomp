/* T5.c1 reproducer (our C), expand-cse: multiply and divide synthesis. expand_mult (expmed.c:2304) asks synth_mult
   (expmed.c:2060) for a shift/add sequence cheaper than mult_cost; a: x * 10 -> shift/add chain, no mult;
   b: x * 0x12345679 -> mult (synth too dear). Signed division by 4 (expand_divmod, expmed.c:2881) adds the round-toward-zero
   bias: c: x / 4 -> bias + sra; d: x >> 2 -> one sra. */
int mul_a(int x) { return x * 10; }
int mul_b(int x) { return x * 0x12345679; }
int mul_c(int x) { return x / 4; }
int mul_d(int x) { return x >> 2; }
