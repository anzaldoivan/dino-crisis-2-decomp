/* T3.c2 reproducer (our C): fused expression vs temp across statements. */
typedef struct { unsigned char b; } S;
int combine_split_fused_a(S *p, int k) { return ((p->b << 2) + k) & 0xff; }
int combine_split_fused_b(S *p, int k) { int t; t = p->b << 2; t += k; return t & 0xff; }
