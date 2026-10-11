#include "common.h"
#include "dc2.h"

typedef struct Obj0x8002953c {
    char pad0[0x2476];
    unsigned char x2476;
    char pad2477[0x247D - 0x2477];
    unsigned char x247D;
} Obj0x8002953c;
extern unsigned short *D_80089CE4[];
extern int func_8001D9D4(void);
extern void func_8001EC10(int);
extern int func_8004587C();

void func_8002953C(Obj0x8002953c *obj)
{
    int id;
    unsigned short v;
    int st;

    v = 0;
    id = func_8004587C();
    if (id != 0xFFFF) {
        v = D_80089CE4[(id & 0xFF00) >> 8][id & 0xFF];
    }
    st = obj->x2476;
    switch (st) {
    case 0:
        if (v != WORK->x218 && id != 0xFFFF) {
            if (!func_8001D9D4()) {
                func_8001EC10(v);
                st = obj->x2476;
                st++;
                obj->x2476 = st;
            }
        } else {
            obj->x2476 = 2;
        }
        break;
    case 1:
        if (!func_8001D9D4()) {
            WORK->x218 = v;
            obj->x2476++;
        }
        break;
    case 2:
        obj->x247D |= 1;
        break;
    }
}
