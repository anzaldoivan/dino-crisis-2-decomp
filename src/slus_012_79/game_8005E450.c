#include "dc2.h"

typedef struct {
    char pad0[0x5F4];
    char *x5F4;
    char pad5f8[0x6EA - 0x5F8];
    unsigned char x6EA;
} Work0x8005e450;
#define WORK0x8005e450 (*SCRATCH(Work0x8005e450 *, 0))
/* Hdr0x8005e450: dc2.h (T8 w35) */
typedef struct {
    int x0;
    char pad4;
    unsigned char x5;
    char pad6;
    unsigned char x7;
    char pad8;
    unsigned char x9;
    char padA[0x8C - 0xA];
} State0x8005e450;
extern Hdr0x8005e450 *D_8008A358;
extern State0x8005e450 D_800B49D0;
extern void *D_800B49C8;
extern void func_80081CA4(void *, int, int);
extern void func_80063CC4(void);
extern void func_8005E854(int, int);
extern void func_8005E518(void);

void func_8005E450(void)
{
    Work0x8005e450 *w = WORK0x8005e450;
    char *p = w->x5F4;

    D_8008A358 = (Hdr0x8005e450 *)p;
    w->x5F4 = p + 0x1C;
    func_80081CA4(&D_800B49D0, 0, 0x8C);
    D_800B49D0.x5 = 0xFF;
    D_800B49D0.x7 = 0xFF;
    D_800B49D0.x9 = 0;
    WORK0x8005e450->x6EA = 0xFF;
    D_800B49D0.x0 = (int)WORK0x8005e450->x5F4 + D_8008A358->xC;
    func_80063CC4();
    func_80081CA4(D_800B49C8, 0, 0x668);
    func_8005E854(0, 0);
    func_8005E518();
}
