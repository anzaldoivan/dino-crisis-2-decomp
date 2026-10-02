#include "common.h"

typedef struct {
    char pad[0x51];
    signed char f51;
    signed char f52;
    signed char f53;
} Obj;
typedef struct {
    char *a;
    char *b;
    int c;
} StrEnt;
typedef struct { char s[0x55]; } Str55;
typedef struct { char s[0x33]; } Str33;
extern StrEnt D_80019770[];
extern Str55 D_80019FC4;
extern Str33 D_8001A01C;
void func_80050D80(void *p, int x, int y, int z, int a, int b, int c);
void func_8004DB98(int a0, int a1, int a2, int a3, int a4, int a5, int a6);
void func_800506BC(char *s, int x, int y, int z, int a, int b, int c);
void func_80050A90(char *s, int x, int y, int z, int a, int b);

void func_80057A8C(Obj *obj)
{
    Str55 s1;
    Str33 s2;

    func_80050D80((void *)0x801857B0, 0x1C, 0x18, 0xC, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x801857C0, 0x8C, 0x18, 0xC, 0x7C8E, 0x80808080, 3);
    func_80050D80((void *)0x801857C8, 0x114, 0x28, 0xC, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x801857D0, 0x24, 0x18, 0xC, 0x7C8F, 0x2060C0, 2);
    func_80050D80((void *)0x801857D8, 0x58, 0x38, 0xC, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x801858D8, 0xE0, 0x38, 0xC, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x80185858, 0x58, 0x90, 0xC, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x80185958, 0xE0, 0x90, 0xC, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x80185408, 0x3C, 0x1E, 0xC, 0x7C8F, 0x808080, 2);
    func_8004DB98(0x78, 0x1B, 2, 1, 0x104080, obj->f52 + obj->f51 + 1, 5);
    func_800506BC(D_80019770[obj->f53].a, 0x48, 0x28, 100, 0x808080, 3, 0);
    func_800506BC(D_80019770[obj->f53].b, 0x20, 0x9F, 100, 0x808080, 3, 0);
    func_80050D80((void *)0x80184C60, 0x58, 0xA0, 0xD, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x80184D90, 0xE0, 0xA0, 0xD, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x80184EC0, 0x58, 0xD2, 0xD, 0x7C8E, 0x80808080, 2);
    func_80050D80((void *)0x80184F40, 0xE0, 0xD2, 0xD, 0x7C8E, 0x80808080, 2);
    s1 = D_80019FC4;
    func_80050A90(s1.s, 0x30, 0x40, 200, 0x606060, 1);
    s2 = D_8001A01C;
    func_80050A90(s2.s, 0xE0, 0x78, 200, 0x104080, 1);
}
