/* multi-set-param before (T4.c1, our C): natural draft of slus_012_79:0x8003E164; the narrowed param n is read once
   as n + 1, so after combine its pseudo has one SET and sched1 sinks the andi/addu to the store (birthing). */
typedef struct { char p[0x140]; int f140; int f144; char p1[0x150 - 0x148]; int f150; int f154; int f158; char p2[0x15f - 0x15c]; char f15f; char f160; char p3[3]; short f164, f166, f168, f16a, f16c, f16e; } S;
void func_8003E164(S *s, int n)
{
    n &= 0xff;
    s->f15f = 7;
    s->f164 = 0x50;
    s->f166 = 0x50;
    s->f150 = 0;
    s->f154 = 0;
    s->f140 = 0;
    s->f144 = 0;
    s->f168 = 0;
    s->f16c = 0;
    s->f158 = n + 1;
    s->f160 = 0;
    s->f16a = 0x200;
    s->f16e = 0x300;
}
