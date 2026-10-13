#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x4E];
    unsigned short x4E;
    int x50;
} Obj0x80037704;
extern unsigned char D_800ABF50[];
extern int D_800A79F0;
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80037694();
extern void func_80043D94(int);
extern void func_800721A4();
extern int func_80075148(int, int, int, int);
extern void func_800782E4(void);
extern void func_80085800();

void func_80037704(Obj0x80037704 *p)
{
    func_80085800(0);
    func_800721A4();
    func_80043D94(0);
    switch (p->x4E) {
    case 0:
    case 10:
        break;
    default:
        func_80037694(p);
        func_8002EB14(1, 1, 1);
        break;
    }
    if (p->x4E != 10) {
        D_800ABF50[0xA1] = 0;
        D_800ABF50[0x11] = 0;
        D_800ABF50[0x2C] = 1;
        D_800ABF50[0xBC] = 1;
    }
    if (func_80034CD8(2, 0x1E) != 0) {
        D_800ABF50[0xA1] = 0;
        D_800ABF50[0x11] = 0;
        D_800ABF50[0x2C] = 1;
        D_800ABF50[0xBC] = 1;
        func_80037694(p);
        func_8002EB14(1, 1, 1);
    }
    func_800782E4();
    while (func_80075148(9, 0, 0, -1) == 0) {
    }
    WORK->x4BA = 0;
    func_80034C48(2, 0x17, 0);
    D_800A79F0 = p->x50;
}
