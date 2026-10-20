#include "common.h"

#include "dc2.h"
typedef struct Grp0x8002f5c8 {
    char pad0[0x10];
    void (*x10)();
    char pad14[4];
} Grp0x8002f5c8;
typedef struct Work0x8002f5c8 {
    char pad0[0x204];
    short x204;
    char pad206[0x21C - 0x206];
    unsigned short x21C;
    char pad21e[0xC2D - 0x21E];
    unsigned char xC2D;
} Work0x8002f5c8;
#define WORK0x8002f5c8 (*SCRATCH(Work0x8002f5c8 *, 0))
extern Grp0x8002f5c8 D_80089B08[];
extern void func_80041778();

void func_8002F5C8(void)
{
    WORK0x8002f5c8->x204++;
    if (WORK0x8002f5c8->x21C != WORK0x8002f5c8->xC2D) {
        func_80041778();
        D_80089B08[WORK0x8002f5c8->xC2D].x10();
    }
}
