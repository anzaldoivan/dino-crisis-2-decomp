#include "common.h"

typedef struct {
    unsigned char *p0;                    
    unsigned char *p4;                    
    int pad8;                             
    union {
        int w;
        unsigned char b[4];
    } uC;                                                       
    int pad10;                            
    unsigned char state;                  
    unsigned char pad15;                  
    unsigned char cnt;                    
} Obj0x8001bf5c;
extern unsigned char D_800A79BA[];

void func_8001BF5C(Obj0x8001bf5c *o)
{
    unsigned char *a1 = o->p4;
    unsigned char *a2 = o->p0;

    switch (o->state) {
    case 0:
        o->state = 1;
        o->uC.b[1] = a2[3];
        o->cnt = a2[1];
        o->uC.b[2] = a1[0];
    case 1:
        if (o->uC.b[2] == 0) {
            if (--o->cnt == 0) {
                o->state++;
                return;
            }
            a1++;
            o->uC.b[2] = *a1;
            o->uC.b[1] ^= 1;
            o->p4 = a1;
        }
        o->uC.b[2]--;
        D_800A79BA[0] = o->uC.b[1];
        return;
    case 2:
        o->uC.w &= ~0x40;
        return;
    }
}
