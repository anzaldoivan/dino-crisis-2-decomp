/* T3.c2 reproducer (our C): s16 cast placement. a = cast at the load (lh), b = load u16 then cast after arithmetic. */
typedef struct { unsigned short h; short s; } S;
int combine_s16_cast_a(S *p) { return (short)p->s + 1; }
int combine_s16_cast_b(S *p) { return (short)(p->h + 1); }
