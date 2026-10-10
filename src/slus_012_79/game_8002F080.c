#include "dc2.h"

typedef struct {
    char pad0[0x3DC];
    short x3DC;
    short x3DE;
    short x3E0;
    short x3E2;
    char pad3e4[0xC2A - 0x3E4];
    signed char xC2A;
    unsigned char xC2B;
    unsigned char xC2C;
    unsigned char xC2D;
    char padc2e[0xC34 - 0xC2E];
    int xC34;
    char padc38[0xC46 - 0xC38];
    char xC46;
    char padc47;
    short xC48;
    short xC4A;
    short xC4C;
    short xC4E;
    short xC50;
    short xC52;
    short xC54;
    short xC56;
    char padc58[0xD94 - 0xC58];
    short xD94;
    char padd96[2];
    short xD98;
    char padd9a[0xDA0 - 0xD9A];
    int xDA0;
} Wk0x8002f080;
typedef struct {
    char pad0[8];
    unsigned short x8;
    unsigned short xA;
    unsigned short xC;
    unsigned short xE;
    char pad10[8];
} Ent0x8002f080;
typedef struct {
    char pad0[0xC];
    Ent0x8002f080 *xC;
    char pad10[8];
} Grp0x8002f080;
extern Grp0x8002f080 D_80089B08[];
extern const Rec0x8001fb5c D_800184C0;
extern void func_80034C48(int, int, int);
extern int func_8004A32C(void);
extern void func_800526B4(void);
#define W ((Wk0x8002f080 *)WORK)

void func_8002F080(void)
{
    Rec0x8001fb5c tbl;
    Ent0x8002f080 *e;
    int i;

    *(unsigned short *)&W->xC2C = 0x101;
    W->xC34 = 0;
    e = &D_80089B08[W->xC2D].xC[W->xC2C];
    W->xC48 = W->x3DC = e->x8;
    W->xC4A = W->x3DE = e->xA;
    W->xC4C = W->x3E0 = e->xC;
    W->xC4E = W->x3E2 = e->xE;
    W->xC2B = 1;
    W->xC50 = 0x101;
    W->xC52 = 0x202;
    W->xC54 = 0x101;
    W->xC56 = 0x50A;
    W->xD94 = 0x4B0;
    W->xD98 = 0x4B0;
    W->xC46 = 1;
    tbl = D_800184C0;
    W->xDA0 = tbl.v[func_8004A32C() & 7] & 0x0F0F0F0F;
    func_800526B4();
    switch (W->xC2A) {
    case 0:
        break;
    case 1:
        func_80034C48(5, 0x55, 1);
        func_80034C48(5, 0x6D, 1);
        func_80034C48(5, 0x4B, 1);
        func_80034C48(5, 0x59, 1);
        func_80034C48(5, 0x4F, 1);
        func_80034C48(5, 0x6E, 1);
        func_80034C48(5, 0x65, 1);
        func_80034C48(5, 0x66, 1);
        func_80034C48(5, 0x67, 1);
        func_80034C48(5, 0x68, 1);
        break;
    case 2:
        for (i = 0x40; i < 0x80; i++) {
            func_80034C48(5, i, 1);
        }
        func_80034C48(5, 0x4A, 0);
        func_80034C48(5, 0x48, 0);
        func_80034C48(5, 0x64, 0);
        func_80034C48(5, 0x5C, 0);
        func_80034C48(5, 0x69, 0);
        func_80034C48(5, 0x6F, 0);
        break;
    }
    func_80034C48(8, 0x19, 1);
}
