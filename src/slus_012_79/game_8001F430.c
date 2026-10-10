#include "common.h"

typedef struct Obj0x8001f430 {
    int *dst;                        
    int blk[8];                      
    int pos[2];                      
    short arr[16];                   
    int v4C;                         
    int v50;                         
    int v54;                         
    int v58;                         
    int v5C;                         
    int v60;                         
    short pad64;                     
    unsigned short v66;              
    unsigned char v68;               
    unsigned char v69;               
    unsigned char v6A;               
    unsigned char pad6B;             
    unsigned char state;             
    unsigned char v6D;               
    unsigned char v6E;               
    unsigned char v6F;               
    short v70;                       
    short v72;                       
    int v74;                         
    int v78;                         
    unsigned char v7C;               
    unsigned char pad7D[11];
    int v88;                         
    unsigned short v8C;              
} Obj0x8001f430;
extern void func_8001E39C(void);
extern void func_8001FB5C();
extern int func_80075148(int, int, int, int);
extern int func_800753BC();
extern void func_800782E4(void);
extern int func_80078324(void);
extern void func_800785A4();
extern void func_800787D4(void *);
extern void func_8001F900();

void func_8001F430(Obj0x8001f430 *s)
{
    unsigned int i;
    int *d;

    switch (s->state) {
    case 0:
        for (i = 0; i < 16; i++) {
            s->arr[i] = 0;
        }
        if (s->v7C & 1) {
            s->v68 = 0;
        } else {
            s->v68 = 1;
        }
        s->v69 = 1;
        s->v58 = s->v60;
        s->v5C = s->v60;
        s->v4C = s->v54;
        s->v50 = s->v54;
        func_800785A4(s->v54, s->pos);
        d = s->dst;
        d[0] = s->blk[0];
        d[1] = s->blk[1];
        d[2] = s->blk[2];
        d[3] = s->blk[3];
        d[4] = s->blk[4];
        d[5] = s->blk[5];
        d[6] = s->blk[6];
        d[7] = s->blk[7];
        s->v78 = 0;
        s->v74 = 0;
        s->v72 = 0;
        s->v70 = 0;
        s->v6F = 0;
        s->v6E = 0;
        s->v6D = 0;
        func_800782E4();
        if (func_80078324() == 1) {
            while (func_800753BC(0xA0, s->pos, 6, func_8001F900, -1) == 0) {
            }
            s->state++;
        }
        break;
    case 1:
        func_8001FB5C();
        if (++s->v6D >= 0x3D) {
            func_800787D4(0);
            while (func_80075148(9, 0, 0, -1) == 0) {
            }
            s->state = 0;
            s->v88++;
            s->v8C++;
            if (++s->v66 >= 3) {
                func_8001E39C();
            }
        } else if (s->v58 <= 0 && *s->dst == 0x6D6D7564) {
            s->state++;
        }
        break;
    case 2:
        while (func_80075148(9, 0, 0, -1) == 0) {
        }
        s->v6A = 0;
        break;
    }
}
