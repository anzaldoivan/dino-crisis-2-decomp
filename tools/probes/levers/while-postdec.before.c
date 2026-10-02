/* while-postdec before (T5.c2, our C): natural draft of slus_012_79:0x800474B4 (walk n links). A counted for whose
   i is used only by the exit test: check_dbra_loop reverses it ("Reversed loop", loop.c:8162), so the counter runs
   down to 0 (blez guard, bnez test) and no constant is needed. */
typedef struct N { char pad[84]; struct N *next; } N;
N *func_800474B4(N *p, int n) { int i; for (i = 0; i < n; i++) p = p->next; return p; }
