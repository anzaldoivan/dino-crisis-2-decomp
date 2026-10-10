#include "dc2.h"

extern void func_8001CDE4(int);
extern void func_8001D9E4(void);
extern void func_80029F7C(void);
extern void func_8002EB14(int, int, int);
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern void func_80044834(void *, int);
extern void func_8004579C();
extern void func_8005AD28();
extern int func_8007E034();
extern void func_8007E114();
extern void func_8007E194();
extern int D_800A79F0;
extern unsigned int *D_800ABA98;

void func_80030DDC(int arg0)
{
    int tp;
    unsigned int *ot;
    unsigned int *ot2;

    if (WORK->x204 != 4) {
        func_80034C48(1, 1, 1);
        D_800A79F0 = 4;
        ((unsigned char *)PKT)[4] = 0x20;
        ((unsigned char *)PKT)[5] = ((unsigned char *)PKT)[6] = 0;
        func_8007E114(PKT);
        ((unsigned char *)PKT)[7] |= 2;
        ((short *)PKT)[4] = 0;
        ((short *)PKT)[5] = 0;
        ((short *)PKT)[6] = 0x140;
        ((short *)PKT)[7] = 0xF0;
        ot = D_800ABA98;
        PKT[0] = (PKT[0] & 0xFF000000) | (ot[29] & 0xFFFFFF);
        ot[29] = (ot[29] & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
        PKT = (void *)((char *)PKT + 0x10);
        tp = func_8007E034(1, 1, 0x140, 0x100);
        func_8007E194(PKT, 1, 1, tp, 0);
        ot2 = D_800ABA98;
        PKT[0] = (PKT[0] & 0xFF000000) | (ot2[29] & 0xFFFFFF);
        ot2[29] = (ot2[29] & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF);
        PKT = (void *)((char *)PKT + 0xC);
        func_8005AD28();
        func_8001CDE4(1);
        WORK->x6F8 = 0x80;
        WORK->x6F9 = 0x80;
        WORK->x6FA = 0x80;
        WORK->x6FB = 0;
        WORK->x6FC = 0;
        func_80029F7C();
        if (arg0 != 0) {
            func_8002EB14(1, arg0, 3);
        } else {
            func_8002EB14(1, 0x32, 3);
        }
        func_8001D9E4();
        func_8004579C();
        if ((unsigned short)(*(unsigned short *)&WORK->xC2C - 0x407) >= 3) {
            if (func_80034CD8(2, 0xB) == 0) {
                func_80044834(0, 4);
            }
        }
        WORK->x204 = 4;
        WORK->x206 = 0;
        WORK->x208 = 0;
        WORK->x20A = 0;
    }
}
