/* lever swap-returning-arm (T4.c2, group sched-reorg/jump), before: natural draft of slus_012_79:0x8005EE58:
 * the arm the game places at fallthrough is written first, as an early-return then-arm. Our C; self-contained (README.md). */
typedef struct { int v; char p[0x84 - 4]; int *ptr; char p2[0x9b - 0x88]; char f9b; char f9c; signed char f9d; } S;

int func_8005EE58(S *s)
{
    if (s->f9d == 0) {
        s->f9b = 0;
        return 1;
    }
    s->f9d--;
    s->v = *s->ptr++;
    return 0;
}
