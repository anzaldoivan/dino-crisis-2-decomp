/* shift-scaled-index after (T5.c1, our C): the one change: scale by << 2 instead of * 4 (an LSHIFT_EXPR, not a
   MULT, so expand keeps base first: addu base,index, the game order). */
typedef struct { char p[0x5ec]; char *f5ec; } G;
typedef struct { char p[0x84]; unsigned char *f84; char p2[0x9f - 0x88]; signed char f9f; } S;
void func_800604F4(S *s)
{
    char *b = (*(G **)0x1F800000)->f5ec;
    char *t = b + *(int *)(b + (s->f9f << 2) + 8);
    if (((int *)s->f84)[1] == 30) {
        t[3] = s->f84[0];
    }
}
