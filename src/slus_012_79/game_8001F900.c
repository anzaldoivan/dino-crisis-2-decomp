#include "common.h"

typedef struct {
    unsigned char pad0[0x66];
    unsigned short unk66;
    unsigned char pad68[0x4];
    unsigned char unk6C;
    unsigned char unk6D;
    unsigned char pad6E[0x1A];
    int unk88;
    unsigned short unk8C;
} Ctx0x8001f900;
extern Ctx0x8001f900 D_800AF5C0;
extern void func_8001E39C(void);
extern void func_800787D4(void *);
extern void func_8001F6BC(void);

void func_8001F900(unsigned char arg0)
{
    Ctx0x8001f900 *p = &D_800AF5C0;

    if (arg0 == 2) {
        func_800787D4(func_8001F6BC);
        p->unk6D = 0;
    } else if (arg0 == 5) {
        p->unk6C = 0;
        p->unk8C++;
        p->unk88++;
        if (++p->unk66 >= 3) {
            func_8001E39C();
        }
    }
}
