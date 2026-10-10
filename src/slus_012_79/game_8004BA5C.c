#include "common.h"
#include "dc2.h"

typedef struct Obj0x8004ba5c {
    char pad0[0x8C];
    signed char state;
    signed char timer;
} Obj0x8004ba5c;
struct Obj0x8004bbf4;
extern Name8 D_80019B48;
extern int func_8004A32C(void);
extern void func_80050030();
extern void func_800513C0();
#define OBJ_8004BA5C ((Obj0x8004ba5c *)arg)

void func_8004BA5C(struct Obj0x8004bbf4 *arg)
{
    Name8 buf;
    unsigned char i;
    unsigned char a;
    unsigned char c;

    switch (OBJ_8004BA5C->state) {
    case 0:
        break;
    case 1:
        OBJ_8004BA5C->timer = 10;
        OBJ_8004BA5C->state++;
    case 2:
        buf = D_80019B48;
        c = OBJ_8004BA5C->timer * 10;
        func_80050030(&buf, (c << 16) | (c << 8) | c, 1, 2);
        for (i = 0; i < OBJ_8004BA5C->timer * 20; i++) {
            a = (unsigned int)func_8004A32C() % 239;
            func_800513C0((unsigned char)((unsigned int)func_8004A32C() % 15) - 8, a, 1);
        }
        if (--OBJ_8004BA5C->timer <= 0) {
            OBJ_8004BA5C->state = 0;
        }
        break;
    }
}
