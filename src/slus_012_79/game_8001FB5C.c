#include "common.h"
#include "dc2.h"

typedef struct Seq0x8001fb5c {
    int *p;                        
    Rec0x8001fb5c cur;             
    int pad24[2];                  
    unsigned short q[16];          
    int pad4C;                     
    int f50;                       
    int f54;                       
    int pad58;                     
    int f5C;                       
    int f60;                       
    int pad64;                     
    unsigned char pad68;           
    unsigned char idx;             
    unsigned char pad6A[2];
    unsigned char f6C;             
    unsigned char pad6D;
    unsigned char f6E;             
    unsigned char f6F;             
    short f70;                     
    short f72;                     
    int f74;                       
    int f78;                       
    unsigned char f7C;             
    unsigned char pad7D[2];
    unsigned char f7F;             
} Seq0x8001fb5c;
extern Seq0x8001fb5c D_800AF5C0;
extern void (*D_800875CC[])(Seq0x8001fb5c *, int *);
extern int D_80087568;
extern void func_8001FA70();
extern void func_80078374();

void func_8001FB5C(void)
{
    Seq0x8001fb5c *s = &D_800AF5C0;
    int *p;

    while (s->q[s->idx] != 0) {
        p = s->p;
        if ((unsigned int)p[0] >= 10) {
            func_8001FA70();
            return;
        }
        D_800875CC[p[0]](s, p);
        s->q[s->idx] = 0;
        s->idx++;
        if (s->idx >= 16) {
            s->idx = 1;
        }
        s->f50++;
        s->f5C -= 0x800;
        s->p[1] -= 0x800;
        if (s->p[1] <= 0) {
            if (s->f7C & 1) {
                s->f7C &= 0xFE;
                s->f54++;
                s->f50++;
                s->f60 -= 0x800;
                s->f5C -= 0x800;
            }
            *(Rec0x8001fb5c *)s->p = s->cur;
            s->f78 = 0;
            s->f74 = 0;
            s->f72 = 0;
            s->f70 = 0;
            s->f6F = 0;
            s->f6E = 0;
            s->f54 = s->f50;
            s->f60 = s->f5C;
            s->p += 8;
            s->cur = *(Rec0x8001fb5c *)s->p;
        }
    }
    if (s->f7F & 0x80) {
        s->f7F &= 0x7F;
        s->f6C = 0;
        func_80078374(D_80087568, 0x200);
    }
}
