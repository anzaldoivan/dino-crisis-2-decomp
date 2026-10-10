#include "common.h"

#include "dc2.h"
typedef struct {
    unsigned short x0;
    unsigned short x2;
    char pad4[0x3C - 0x4];
    short x3C;
    short x3E;
} Obj_800375D4;
extern unsigned short D_800ABA90;
int func_800782A4(void);
void func_80034C48(int, int, int);
void func_80037824(int, int);
void func_80037B7C(Obj_800375D4 *);

void func_800375D4(Obj_800375D4 *p)
{
    if (func_800782A4() & 0x10) {
        WORK->x4BA = 2;
    }
    if (D_800ABA90 & p->x2) {
        WORK->x4BA = 2;
        func_80034C48(2, 0x1E, 1);
    }
    if (WORK->x4BA == 2) {
        p->x0++;
    } else {
        func_80037824(p->x3C, p->x3E);
        func_80037B7C(p);
    }
}
