/* T3.c2 reproducer (our C): u16 vs s32 temp held across a use. a = u16 temp, b = int temp. */
typedef struct { unsigned short h; } S;
extern void use(int);
int combine_u16_temp_a(S *p) { unsigned short t = p->h; use(t); return t + 1; }
int combine_u16_temp_b(S *p) { int t = p->h; use(t); return t + 1; }
int combine_u16_temp_c(S *p) { unsigned short t = p->h + 1; use(t); return t; }
int combine_u16_temp_d(S *p) { int t = p->h + 1; use(t); return t; }
