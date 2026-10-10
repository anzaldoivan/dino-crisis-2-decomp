#include "common.h"
#include "dc2.h"
#include "../shared/func_800436C8.h"
#include "../shared/func_8002B3E0.h"

/* func_80037824 */

extern Pair D_800893D0;
extern unsigned char D_800ABA96;

/* func_80037E18: Unit, Work, WORK in dc2.h */

extern int D_800AF11C;




extern unsigned char D_800AE547;



char *func_800474B4(int a, int b);
void func_8004A240(void *, void *, SVECTOR *);



#define SHARED_D0 D_800894C8

#define SHARED_F0 func_800487B8

extern void (*D_800899E0[])(void *);
extern void (*D_80089A30[])(void *);


extern void (*D_80089A58[])(void *);


extern void (*D_80089AB8[])(void *);


extern void (*D_80089AE0[])(void *);


extern void func_80047958(void *);



extern Pair D_800B2890[];
void func_80018020(void *, int);
unsigned int func_8003252C(void *);
void func_800339AC(void *, Seg *, SVECTOR *);
int func_80033D4C(void *, void *, SVECTOR *, int);
int func_8003DD34(int, int, unsigned short);
int func_8003F154(void *, void *, int);
void func_80044834(void *, int);
char *func_800474B4(int a, int b);
void func_80047958(void *);
void func_80048048(void *, void *, int, int, int, int);
void func_8004A240(void *, void *, SVECTOR *);
int func_8004A32C(void);
void func_800649A8(int, int, int);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007AA34(SVECTOR *, MATRIX *);


extern void func_80018020(void *, int);


INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800412C8", func_800412C8);

INCLUDE_ASM("asm/slus_012_79/nonmatchings/game_800412C8", func_80041338);

typedef struct Ent0x80041778 {
    int x0;
    unsigned short x4;
    char pad6[0x18 - 0x6];
} Ent0x80041778;
extern Ent0x80041778 D_80089B08[];
extern int D_800181D4;
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001ECBC(int, int);

void func_80041778(void)
{
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
    func_8001ECBC(D_80089B08[WORK->xC2D].x4, D_800181D4);
    while (func_8001D9D4()) {
        func_8001CDE4(1);
    }
}

extern unsigned short *D_80089CE4[];
extern void func_8001CDE4(int);
extern int func_8001D9D4(void);
extern void func_8001EC10(int);




extern void func_80045C18(int arg0);


extern void func_80043E1C(int);


extern void func_80045A20(int, int);


extern signed char D_800B32FB;
extern void func_80044A88(int, int, int, int);



extern Elem28 D_800B4A68[];


extern int D_800B4F74;


extern S800B53A8 D_800B53A8;
extern unsigned short D_800ABA68;


