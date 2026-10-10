/* reuse-user-var before (T3.c1, our C): natural draft of slus_012_79:0x8005ED34; bump through memory, s->c++. */
typedef struct { unsigned char p0, f1, f2, p3; short f4; } C;
typedef struct { C *c; char p[0x84]; int tab[1]; } S;
extern void func_8005EAB4(S *, int, int, int);
extern void func_8005EC80(S *, int);
int func_8005ED34(S *s)
{
    C *c = s->c;
    int x = s->tab[c->f1];
    func_8005EAB4(s, c->f2, c->f4, x);
    func_8005EC80(s, x);
    s->c++;
    return 0;
}
