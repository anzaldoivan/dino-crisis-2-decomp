#include "common.h"

typedef struct {
    unsigned char *pc;
    char pad04[0x84];
    int regs[1];
} Ctx0x8005ed34;
extern void func_8005EAB4(Ctx0x8005ed34 *ctx, int a, int b, int c);
extern void func_8005EC80(Ctx0x8005ed34 *ctx, int v);

int func_8005ED34(Ctx0x8005ed34 *ctx)
{
    unsigned char *pc = ctx->pc;
    int v = ctx->regs[pc[1]];
    func_8005EAB4(ctx, pc[2], *(short *)(pc + 4), v);
    func_8005EC80(ctx, v);
    pc = ctx->pc;
    pc += 6;
    ctx->pc = pc;
    return 0;
}
