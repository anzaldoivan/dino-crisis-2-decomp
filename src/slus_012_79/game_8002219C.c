#include "common.h"

typedef struct { char s[8]; } Str8;
extern Str8 D_80018268;
extern Str8 D_80018270;
int func_80034CD8(int, int);
void func_8002204C(int, int, int, int);
void func_80021F78(Str8 *, int, int, int, int, int, int);
void func_80034C48(int, int, int);

void func_8002219C(void)
{
    Str8 buf1;
    Str8 buf2;
    int t;
    int m;
    int color;
    unsigned int v;

    if (func_80034CD8(1, 14) == 0) {
        (*(int **)0x1F800000)[0xE64 / 4]--;
    }
    m = (*(int **)0x1F800000)[0xE68 / 4];
    t = (*(int **)0x1F800000)[0xE64 / 4];
    if (t < m / 10) {
        color = 0x101080;
    } else if (t < m / 2) {
        color = 0x108080;
    } else {
        color = 0x108000;
    }
    v = (unsigned int)t / 1800;
    func_8002204C(100, 50, v, color);
    t -= v * 1800;
    v = (unsigned int)t / 30;
    buf1 = D_80018268;
    func_80021F78(&buf1, 64, 64, 0x7DCD, color, 0, 1);
    func_8002204C(0x8C, 50, v, color);
    t -= v * 30;
    t = t * 3;
    buf2 = D_80018270;
    func_80021F78(&buf2, 64, 64, 0x7DCD, color, 0, 1);
    func_8002204C(0xB4, 50, t, color);
    if ((*(int **)0x1F800000)[0xE64 / 4] <= 0) {
        func_80034C48(3, 9, 0);
        func_80034C48(2, 11, 1);
    }
}
