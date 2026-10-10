/* T3.c1 reproducer (our C): new temp vs reused user var for a late pointer bump. a: s->c++ (fresh pseudos);
   b: c = s->c; c++; s->c = c; (user var c set 3 times). c's early life overlaps a $2 temp (the t[] index). */
typedef struct { unsigned char p, a, b, q; short h; } C;
typedef struct { C *c; int t[4]; } S;
extern void g(S *, int, int, int);
extern void k(S *, int);
int reuse_a(S *s) { C *c = s->c; int x = s->t[c->a]; g(s, c->b, c->h, x); k(s, x); s->c++; return 0; }
int reuse_b(S *s) { C *c = s->c; int x = s->t[c->a]; g(s, c->b, c->h, x); k(s, x); c = s->c; c++; s->c = c; return 0; }
