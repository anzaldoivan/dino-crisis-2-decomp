#include "common.h"

#include "dc2.h"
typedef struct {
    char pad0[0x2474];
    unsigned char x2474;
    char pad2475[0x2477 - 0x2475];
    unsigned char x2477;
    unsigned short x2478;
    char pad247A[0x247D - 0x247A];
    unsigned char x247D;
    char pad247E[0x2498 - 0x247E];
    char *x2498;
} Obj0x80027dcc;
extern int func_80034CD8(int, int);
extern void func_80064A60();
extern void func_80064AB4(void);
extern int func_80065D0C(int, int, char *, int);
extern char D_8008ED62[];
extern char D_8008EDB8[];
extern int D_800AF11C;

void func_80027DCC(Obj0x80027dcc *p)
{
    int st;

    switch (st = p->x2477) {
    case 0:
        func_80064A60();
        if (p->x2478 == 1) {
            p->x2498 = D_8008ED62;
        } else {
            p->x2498 = D_8008EDB8;
        }
        func_80065D0C(0, 0, p->x2498, 2);
        st = p->x2477;
        st++;
        p->x2477 = st;
        D_800AF11C = 0;
    case 1:
        func_80064AB4();
        if (func_80034CD8(2, 0x1F) != 0) {
            break;
        }
        p->x2477++;
    case 2:
        if (p->x247D == 0x11) {
            p->x2474++;
        }
        break;
    }
}
