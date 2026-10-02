/* shift-scaled-index before (T5.c1, our C): natural draft of slus_012_79:0x800604F4; the table index is scaled
   by a multiply (x 4), so expand_expr PLUS_EXPR (expr.c:7035) puts the MULT first: addu index,base (operands
   swapped vs the game). */
typedef struct { char p[0x5ec]; char *f5ec; } G;
typedef struct { char p[0x84]; unsigned char *f84; char p2[0x9f - 0x88]; signed char f9f; } S;
void func_800604F4(S *s)
{
    char *b = (*(G **)0x1F800000)->f5ec;
    char *t = b + *(int *)(b + s->f9f * 4 + 8);
    if (((int *)s->f84)[1] == 30) {
        t[3] = s->f84[0];
    }
}
