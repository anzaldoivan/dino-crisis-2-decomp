#include "dc2.h"

typedef struct {
    unsigned short cur;
    unsigned short trig;
    unsigned short old;
    unsigned short stat;
    short ang;
    unsigned short mag;
    unsigned short xC;
    unsigned short xE;
} Pad0x8001c854;
typedef struct {
    unsigned char x0[2];
    unsigned char b2;
    unsigned char b3;
    unsigned char x4[2];
    unsigned char b6;
    unsigned char b7;
    unsigned char x8[0x1C];
} Raw0x8001c854;
typedef struct {
    short x, y, z;
} Vec0x8001c854;
extern Pad0x8001c854 D_800AF130;
extern Pad0x8001c854 D_800AF140;
extern Raw0x8001c854 D_800ABAA0[];
extern unsigned char D_800A79B8;
extern unsigned short D_800A79C8[];
extern unsigned short D_800A79D8[];
extern unsigned short D_800ABA68;
extern unsigned short D_800ABA6A;
extern unsigned short D_800ABA90;
extern unsigned short D_800ABA92;
extern char D_80085F98[];
#define SCR08_0x8001c854 (*SCRATCH(unsigned short, 0x8))
#define SCR0A_0x8001c854 (*SCRATCH(unsigned short, 0xA))
extern void func_8001C2B0();
extern void func_8001C72C();
extern void func_80034C48(int, int, int);
extern int func_80034CD8(int, int);
extern int func_8004A2C4(void *p, int arg);
extern int func_80078E14(int);
extern void func_800827B4();
extern int func_80082924();
extern int func_800829F4();

void func_8001C854(void)
{
    Vec0x8001c854 dir;
    Vec0x8001c854 org;
    Pad0x8001c854 *p;
    Raw0x8001c854 *raw;
    unsigned int i;
    int port;
    unsigned int type;
    int mode;
    unsigned short btn;
    short ang;
    int dx, dz;

    func_8001C2B0();
    org.z = 0;
    org.y = 0;
    org.x = 0;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            port = 0;
            p = &D_800AF130;
        } else {
            port = 0x10;
            p = &D_800AF140;
        }
        raw = &D_800ABAA0[i];
        type = func_80082924(port);
        if (type < 2) {
        none:
            p->xE = 0;
            p->xC = 0;
            p->mag = 0;
            p->ang = 0;
            p->trig = 0;
            p->cur = 0;
            p->old = 0;
            p->stat = 4;
            if (i == 0) {
                D_800A79B8 = 0;
                if (func_80034CD8(2, 6) == 0 && func_80034CD8(2, 0xF) == 0
                    && func_80034CD8(2, 0x1F) == 0 && type == 0) {
                    func_80034C48(2, 0x10, 1);
                }
            }
            continue;
        }
        mode = func_800829F4(port, 1, 0);
        p->stat = 0;
        switch (mode) {
        case 4:
            btn = ~((raw->b2 << 8) | raw->b3);
            p->xE = 0;
            p->xC = 0;
            p->mag = 0;
            p->ang = 0;
            break;
        case 7:
            p->stat = 1;
            dx = raw->b6 - 0x80;
            dz = 0x80 - raw->b7;
            dir.y = 0;
            dir.x = dx;
            dir.z = dz;
            btn = func_80078E14(dx * dx + dz * dz);
            p->mag = 0;
            if (btn > 0x34) {
                p->mag = btn;
                p->ang = ang = func_8004A2C4(&org, (int)&dir);
                if (ang < 0x100) {
                    btn = 0x1000;
                } else if (ang < 0x300) {
                    btn = 0x3000;
                } else if (ang < 0x500) {
                    btn = 0x2000;
                } else if (ang < 0x700) {
                    btn = 0x6000;
                } else if (ang < 0x900) {
                    btn = 0x4000;
                } else if (ang < 0xB00) {
                    btn = 0xC000;
                } else if (ang < 0xD00) {
                    btn = 0x8000;
                } else if (ang < 0xF00) {
                    btn = 0x9000;
                } else {
                    btn = 0x1000;
                }
                btn |= ~((raw->b2 << 8) | raw->b3) & 0xFFF;
            } else {
                btn = ~((raw->b2 << 8) | raw->b3);
            }
            break;
        default:
            goto none;
        }
        if (type == 6) {
            p->stat |= 2;
        }
        if (D_800A79B8 == 0 && i == 0) {
            if (type == 2) {
                D_800A79B8 = 1;
            } else if (type == 6) {
                func_800827B4(port, D_80085F98);
                D_800A79B8 = 1;
            }
        }
        {
            unsigned short o = p->cur;
            p->cur = btn;
            p->old = o;
            p->trig = btn & ~o;
        }
    }
    func_8001C72C(&D_800AF130, D_800A79C8);
    D_800ABA68 = D_800A79C8[0];
    D_800ABA90 = D_800A79C8[1];
    func_8001C72C(&D_800AF140, D_800A79D8);
    D_800ABA6A = D_800A79D8[0];
    D_800ABA92 = D_800A79D8[1];
    if (func_80034CD8(1, 0)) {
        D_800ABA92 = 0;
        D_800ABA6A = 0;
        SCR0A_0x8001c854 = 0;
        SCR08_0x8001c854 = 0;
    } else {
        SCR08_0x8001c854 = D_800ABA68;
        SCR0A_0x8001c854 = D_800ABA90;
    }
}
