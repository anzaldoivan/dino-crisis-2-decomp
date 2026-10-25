#include "dc2.h"

typedef struct {
    unsigned char pad0[3];
    unsigned char len;
    unsigned int code;
    unsigned int w2;
} Mode0x800310b8;
typedef struct {
    unsigned int tag;
    unsigned int code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} Ft40x800310b8;
extern OtHolder0x800310b8 *D_800ABA98;
extern unsigned char D_800ABA96;
extern unsigned short D_80089214;
#define FT4 ((Ft40x800310b8 *)PKT)

void func_800310B8(void)
{
    ((Mode0x800310b8 *)PKT)->len = 2;
    ((Mode0x800310b8 *)PKT)->code = 0xE6000000;
    ((Mode0x800310b8 *)PKT)->w2 = 0;
    { OtHolder0x800310b8 *h = D_800ABA98;
    *PKT = (*PKT & 0xFF000000) | (h->x78 & 0xFFFFFF);
    h->x78 = (h->x78 & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF); }
    PKT = (unsigned int *)((char *)PKT + 0xC);

    {
    FT4->x0 = FT4->x2 = D_80089214;
    FT4->x1 = FT4->x3 = 0xA0;
    FT4->y0 = FT4->y1 = D_80089214;
    FT4->y2 = FT4->y3 = 0x78;
    FT4->u0 = FT4->u2 = 0;
    FT4->u1 = FT4->u3 = 0xA0;
    FT4->v0 = FT4->v1 = 0;
    FT4->v2 = FT4->v3 = 0x78;
    FT4->tpage = D_800ABA96 == 0 ? 0x110 : 0x100;
    { unsigned int *c = PKT + 1; *c = 0x2E808080; } }
    { OtHolder0x800310b8 *h = D_800ABA98; unsigned int *p = PKT;
    *p = h->x78 | 0x09000000;
    h->x78 = (unsigned int)p & 0xFFFFFF; }
    PKT = (unsigned int *)((char *)PKT + 0x28);

    {
    FT4->x0 = FT4->x2 = 0xA0;
    FT4->x1 = FT4->x3 = 0x140 - D_80089214;
    FT4->y0 = FT4->y1 = D_80089214;
    FT4->y2 = FT4->y3 = 0x78;
    FT4->u0 = FT4->u2 = 0x20;
    FT4->u1 = FT4->u3 = 0xC0;
    FT4->v0 = FT4->v1 = 0;
    FT4->v2 = FT4->v3 = 0x78;
    FT4->tpage = D_800ABA96 == 0 ? 0x112 : 0x102;
    { unsigned int *c = PKT + 1; *c = 0x2E808080; } }
    { OtHolder0x800310b8 *h = D_800ABA98; unsigned int *p = PKT;
    *p = h->x78 | 0x09000000;
    h->x78 = (unsigned int)p & 0xFFFFFF; }
    PKT = (unsigned int *)((char *)PKT + 0x28);

    {
    FT4->x0 = FT4->x2 = D_80089214;
    FT4->x1 = FT4->x3 = 0xA0;
    FT4->y0 = FT4->y1 = 0x78;
    FT4->y2 = FT4->y3 = 0xF0 - D_80089214;
    FT4->u0 = FT4->u2 = 0;
    FT4->u1 = FT4->u3 = 0xA0;
    FT4->v0 = FT4->v1 = 0x78;
    FT4->v2 = FT4->v3 = 0xF0;
    FT4->tpage = D_800ABA96 == 0 ? 0x110 : 0x100;
    { unsigned int *c = PKT + 1; *c = 0x2E808080; } }
    { OtHolder0x800310b8 *h = D_800ABA98; unsigned int *p = PKT;
    *p = h->x78 | 0x09000000;
    h->x78 = (unsigned int)p & 0xFFFFFF; }
    PKT = (unsigned int *)((char *)PKT + 0x28);

    {
    FT4->x0 = FT4->x2 = 0xA0;
    FT4->x1 = FT4->x3 = 0x140 - D_80089214;
    FT4->y0 = FT4->y1 = 0x78;
    FT4->y2 = FT4->y3 = 0xF0 - D_80089214;
    FT4->u0 = FT4->u2 = 0x20;
    FT4->u1 = FT4->u3 = 0xC0;
    FT4->v0 = FT4->v1 = 0x78;
    FT4->v2 = FT4->v3 = 0xF0;
    FT4->tpage = D_800ABA96 == 0 ? 0x112 : 0x102;
    { unsigned int *c = PKT + 1; *c = 0x2E808080; } }
    { OtHolder0x800310b8 *h = D_800ABA98; unsigned int *p = PKT;
    *p = h->x78 | 0x09000000;
    h->x78 = (unsigned int)p & 0xFFFFFF; }
    PKT = (unsigned int *)((char *)PKT + 0x28);

    ((Mode0x800310b8 *)PKT)->len = 2;
    ((Mode0x800310b8 *)PKT)->code = 0xE6000001;
    ((Mode0x800310b8 *)PKT)->w2 = 0;
    { OtHolder0x800310b8 *h = D_800ABA98;
    *PKT = (*PKT & 0xFF000000) | (h->x78 & 0xFFFFFF);
    h->x78 = (h->x78 & 0xFF000000) | ((unsigned int)PKT & 0xFFFFFF); }
    PKT = (unsigned int *)((char *)PKT + 0xC);
}
