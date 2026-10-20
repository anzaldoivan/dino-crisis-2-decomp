#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x20];
    MATRIX x20;
    unsigned short x40;
    unsigned short x42;
    unsigned short x44;
    char pad46[2];
    SVECTOR x48;
    unsigned int x50;
    unsigned char x54;
    char pad55[0xE0 - 0x55];
    unsigned short xE0;
    unsigned short xE2;
    unsigned short xE4;
    char padE6[0xF0 - 0xE6];
    unsigned char xF0[4];
} Obj0x8002b228;
extern void (*D_80088DF8[])(Obj0x8002b228 *);
extern void func_80079AA4(SVECTOR *r, MATRIX *m);
extern void func_80049E44(Obj0x8002b228 *, void *);
extern void func_8004AC08();
extern void func_8004AB14(Obj0x8002b228 *p);
extern void func_80047AA0(Obj0x8002b228 *);

void func_8002B228(Obj0x8002b228 *p)
{
    unsigned int m;

    p->xE0 = p->x40;
    m = p->x42;
    p->xE2 = m;
    p->xE4 = p->x44;
    D_80088DF8[p->x54](p);
    func_80079AA4(&p->x48, &p->x20);
    func_80049E44(p, p->xF0);
    func_8004AC08(p);
    func_8004AB14(p);
    m = p->x50;
    m &= 0x40000000;
    if (m) {
        func_80047AA0(p);
    }
}
