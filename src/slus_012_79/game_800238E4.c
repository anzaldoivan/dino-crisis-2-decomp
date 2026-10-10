#include "common.h"

#include "dc2.h"
typedef struct {
    u32 tag;
    u32 code;
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} Tile;
extern OtHolder *D_800ABA98;

void func_800238E4(u16 *rect, u32 color, u16 abe)
{
    u32 *p;
    OtHolder *h;

    ((Tile *)PKT)->x = rect[0];
    ((Tile *)PKT)->y = rect[1];
    ((Tile *)PKT)->w = rect[2];
    ((Tile *)PKT)->h = rect[3];
    if (abe) {
        ((Tile *)PKT)->code = color | 0x62000000;
    } else {
        ((Tile *)PKT)->code = color | 0x60000000;
    }
    h = D_800ABA98;
    p = PKT;
    *p = h->ot | 0x03000000;
    h->ot = (u32)p & 0xFFFFFF;
    PKT = (u32 *)((u8 *)PKT + 0x10);
}
