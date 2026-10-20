#include "common.h"

#include "dc2.h"
extern unsigned char D_800ABA96;
extern void func_80034C48(int, int, int);
extern int func_8007B62C(Rect0x8006c2d8 *, void *);
extern int func_8007B68C(Rect0x8006c2d8 *, void *);
extern void func_80085564(int);
extern void func_800856BC(void *, int);
extern void func_80085738(void *, int);
extern void func_80085794(int);
extern int func_80085800();
extern void func_80085C24(void *, void *);

void func_8001DA44(char *buf)
{
    Rect0x8006c2d8 r;
    char *q;
    char *p;
    unsigned int i;
    Work *w;

    func_80085564(0);
    func_80085800(0);
    p = buf + 0x23A00;
    q = buf + 0x8000;
    func_80085C24(buf, q);
    func_800856BC(q, 0);
    r.x = 0;
    r.y = (D_800ABA96 == 0) << 8;
    r.w = 16;
    r.h = 240;
    for (i = 0; i < 20;) {
        func_80085738(p, 0x780);
        func_80085794(0);
        func_8007B62C(&r, p);
        i++;
        r.x += 16;
    }
    r.x = 0;
    r.w = 0x140;
    func_8007B68C(&r, D_801C1500);
    if (WORK->x204 != 4) {
        func_80034C48(3, 0, 1);
    }
    w = WORK;
    w->x8 = D_801C1500;
}
