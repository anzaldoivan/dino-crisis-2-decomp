/* while-postdec after (T5.c2, our C): the one change: while (n--). The post-decrement leaves n - 1 tested against -1:
   beqz guard, the -1 constant hoisted out of the loop by move_movables (" moved to %d", loop.c:1876), bne test. */
typedef struct N { char pad[84]; struct N *next; } N;
N *func_800474B4(N *p, int n) { while (n--) p = p->next; return p; }
