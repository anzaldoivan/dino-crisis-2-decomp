#include "common.h"

typedef struct Obj0x80026954 {
    char pad0[0x2474];
    unsigned char x2474;
    char pad2475[2];
    unsigned char x2477;
    char pad2478[5];
    unsigned char x247D;
    unsigned char x247E;
} Obj0x80026954;
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80044834(void *, int);

void func_80026954(Obj0x80026954 *obj)
{
    switch (obj->x2477) {
    case 0:
        func_8002EB14(0, 0x14, 1);
        obj->x2477++;
        break;
    case 5:
        if (obj->x247E == 0) {
            break;
        }
        func_8002EB14(1, 0x14, 1);
        obj->x2477++;
        /* fallthrough */
    case 6:
        if (func_80034CD8(2, 6) == 0) {
            func_80034C48(1, 3, 1);
            obj->x2477++;
        }
        break;
    case 7:
        if (obj->x247D == 0x11) {
            obj->x2474++;
            func_80044834(0, 0x79);
        }
        break;
    }
}
