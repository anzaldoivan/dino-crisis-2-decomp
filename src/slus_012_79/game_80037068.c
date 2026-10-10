#include "dc2.h"

typedef struct {
    char pad0[4];
    short x4;
    short x6;
    char pad8[0xF - 0x8];
    unsigned char xF;
    unsigned short x10;
    char pad12[0x14 - 0x12];
} Rec0x80037068;
typedef struct {
    char pad0[2];
    unsigned short x2;
    char pad4[0x3C - 0x4];
    short x3C;
    short x3E;
    char pad40[0x4C - 0x40];
    short x4C;
    short x4E;
    int x50;
} Obj0x80037068;
extern Obj0x80037068 D_800B24A0;
extern Rec0x80037068 D_800892F4[];
extern int D_800A79F0;
extern int D_800AF128;
extern void func_80018020(void *, int);
extern void func_8001CDE4(int);
extern void func_8001D9E4(void);
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern void func_80037824(int, int);
extern void func_80043CC4(int);
extern void func_80043D94(int);

void func_80037068(int arg0)
{
    Rec0x80037068 *r;

    func_8001D9E4();
    func_80043CC4(2);
    func_80043D94(1);
    func_80034C48(2, 0x17, 1);
    func_80034C48(2, 0x1E, 0);
    func_80034C48(3, 0, 0);
    func_8001CDE4(2);
    func_8002EB14(0, 1, 1);
    func_80018020(&D_800B24A0, 0x15);
    D_800B24A0.x50 = D_800A79F0;
    WORK->x4BA = 1;
    WORK->x4BC = 0;
    r = &D_800892F4[arg0];
    D_800B24A0.x4E = arg0;
    D_800A79F0 = r->xF;
    D_800B24A0.x3C = r->x4;
    D_800B24A0.x3E = r->x6;
    func_80037824(D_800B24A0.x3C, D_800B24A0.x3E);
    D_800B24A0.x4C = 0;
    D_800B24A0.x2 = r->x10;
    WORK->x4B9 = 0;
    D_800AF128++;
}
