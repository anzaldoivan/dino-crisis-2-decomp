#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x7C];
    u32 ot;
} OtHolder;
typedef struct {
    u32 tag;
    u32 code;
    u16 x;
    u16 y;
    u8 u;
    u8 v;
    u16 clut;
    u16 w;
    u16 h;
} Sprt;
extern OtHolder *D_800ABA98;

void func_80021F78(u16 *rect, u8 u, u8 v, u16 clut, u32 color, u16 abe)
{
    u32 *p;
    OtHolder *h;

    ((Sprt *)PKT)->x = rect[0];
    ((Sprt *)PKT)->y = rect[1];
    ((Sprt *)PKT)->w = rect[2];
    ((Sprt *)PKT)->h = rect[3];
    ((Sprt *)PKT)->u = u;
    ((Sprt *)PKT)->v = v;
    ((Sprt *)PKT)->clut = clut;
    if (abe) {
        ((Sprt *)PKT)->code = color | 0x66000000;
    } else {
        ((Sprt *)PKT)->code = color | 0x64000000;
    }
    h = D_800ABA98;
    p = PKT;
    *p = h->ot | 0x04000000;
    h->ot = (u32)p & 0xFFFFFF;
    PKT = (u32 *)((u8 *)PKT + 0x14);
}
