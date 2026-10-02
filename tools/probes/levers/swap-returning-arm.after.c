/* lever swap-returning-arm (T4.c2, group sched-reorg/jump), after: test inverted and arms swapped, so jump.c:1796
 * moves the other arm last and the game's fallthrough arm comes first. Game fn slus_012_79:0x8005EE58. Our C. */
typedef struct { int v; char p[0x84 - 4]; int *ptr; char p2[0x9b - 0x88]; char f9b; char f9c; signed char f9d; } S;

int func_8005EE58(S *s)
{
    if (s->f9d != 0) {
        s->f9d--;
        s->v = *s->ptr++;
        return 0;
    }
    s->f9b = 0;
    return 1;
}
