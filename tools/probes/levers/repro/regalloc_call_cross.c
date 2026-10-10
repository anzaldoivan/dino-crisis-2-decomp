/* T3.c1 reproducer (our C): s-reg order of call-crossing params. a: plain (equal priority, ties by birth order);
   b: input-only asm anchors add refs to c (x2) and b (x1), raising their priority. */
extern short f0(void);
extern void f1(int, int, int);
void cross_a(int a, int b, int c, short *t) { t[1] = 0; t[0] = 0; t[2] = f0(); f1(a, b, c); }
void cross_b(int a, int b, int c, short *t) { __asm__("" :: "r"(c), "r"(c), "r"(b)); t[1] = 0; t[0] = 0; t[2] = f0(); f1(a, b, c); }
