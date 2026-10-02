#include "common.h"

typedef struct Obj {
    char pad0[0x50];
    int x50;
    unsigned char x54;
    char pad55[0x60 - 0x55];
    int x60;
    char pad64[0x108 - 0x64];
    short x108;
    char pad10A[2];
    short x10C;
    char pad10E[0x128 - 0x10E];
    void *x128;
    void *x12C;
    char pad130[0x1C0 - 0x130];
    unsigned short x1C0;
} Obj;
extern char D_800DAE48[];
extern char D_800DAE54[];
void func_80047B8C(Obj *, int);
void func_8004A4D4(Obj *, int, int, int);
void func_800484F0(Obj *, int, int, int, int);

void func_800DA040(Obj *s)
{
    s->x50 = 0x20005;
    s->x108 = 0x4B0;
    s->x54++;
    func_80047B8C(s, s->x60);
    func_8004A4D4(s, 8, 0x1000, 0x1000);
    func_800484F0(s, 1, 0, 1, 0);
    s->x12C = D_800DAE54;
    s->x128 = D_800DAE48;
    s->x10C = s->x1C0;
}
