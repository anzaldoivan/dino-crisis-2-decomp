#include "common.h"

#include "dc2.h"
typedef struct {
    int pad0;
    int src;
    unsigned int cur;
    int aux;
    int pad10[4];
} Req0x80020494;
typedef struct {
    char pad0[0x4];
    int src;
    unsigned short x;
    unsigned short y;
    unsigned short limit;
    unsigned short h;
} Arg0x80020494;
typedef struct {
    char pad0[0x6E];
    unsigned char init;
    char pad6F;
    unsigned short y;
    unsigned short x;
    unsigned int cur;
    int aux;
} Obj0x80020494;
extern void func_80020350();
extern int func_8007B62C(Box0x8002243c *, void *);
extern int func_8007B380(int);

void func_80020494(Obj0x80020494 *obj, Arg0x80020494 *arg)
{
    Box0x8002243c rect;
    Req0x80020494 req;
    unsigned int base = 0x801C1500;
    unsigned int n;
    unsigned int addr;

    if (obj->init == 0) {
        obj->cur = base;
        obj->aux = 0;
        obj->init++;
    }
    req.src = arg->src;
    req.cur = obj->cur;
    req.aux = obj->aux;
    func_80020350(obj, &req);
    addr = ((obj->cur - base) >> 11 << 11) + base;
    n = req.cur - addr;
    n >>= 11;
    obj->cur = req.cur;
    obj->aux = req.aux;
    for (; n != 0; n--) {
        rect.w = 32;
        rect.x = obj->x + arg->x;
        rect.y = obj->y + arg->y;
        rect.h = arg->h - obj->y;
        if (rect.h > 32) {
            rect.h = 32;
        }
        func_8007B62C(&rect, (void *)addr);
        obj->x += 32;
        if (obj->x >= arg->limit) {
            obj->x = 0;
            obj->y += 32;
        }
        func_8007B380(0);
        addr += 0x800;
    }
}
