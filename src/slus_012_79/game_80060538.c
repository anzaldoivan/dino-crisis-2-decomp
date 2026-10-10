#include "common.h"

typedef struct Ctx0x80060538 {
    unsigned char *pc;
    char pad04[0x88 - 0x4];
    int regs[5];
    char pad9C[0x9E - 0x9C];
    unsigned char sel;
} Ctx0x80060538;
extern int (*D_8008A38C[])(void);
extern void func_8005EC80(Ctx0x80060538 *ctx, int v);

int func_80060538(Ctx0x80060538 *ctx)
{
    int v;

    v = D_8008A38C[ctx->sel]();
    ctx->regs[0] = v;
    func_8005EC80(ctx, v);
    ctx->pc += 2;
    return 0;
}
