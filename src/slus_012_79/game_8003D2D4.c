#include "common.h"
#include "dc2.h"

typedef struct {
    char pad0[0x40];
    unsigned short x;                
    char pad42[2];
    unsigned short y;                
    char pad46[2];
    SVECTOR rot;                     
    int flags;                       
    char pad54[6];
    short layer;                     
    char pad5C[0xBE];
    unsigned char hit;                
} Obj8003D2D4;
typedef struct {
    unsigned char flags;          
    unsigned char count;          
    unsigned char pad2;
    unsigned char mask;           
    unsigned short layers;        
    char pad6[2];
    unsigned char attr[1];        
} Area;
int func_80032908(Seg *a, Seg *b);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007A824(SVECTOR *r, MATRIX *m);
void func_8007AFA4(VECTOR *a, VECTOR *b, VECTOR *c);

int func_8003D2D4(Obj8003D2D4 *obj, int len)
{
    Seg box;
    Seg seg;
    MATRIX m;
    VECTOR a;
    VECTOR b;
    int mode;
    unsigned int i;
    unsigned int j;
    Area *e;
    unsigned char *attr;
    int *p;

    mode = 0x40;
    if ((void *)obj == WORK->units) {
        mode = 0x80;
    }
    if (obj->flags & 0x1000) {
        mode = 0x20;
    }
    *(int *)&seg.x0 = 0;
    seg.x1 = len;
    func_8007A824(&obj->rot, &m);
    func_80079704(&m, (SVECTOR *)&seg, (SVECTOR *)&seg);
    box.x0 = obj->x;
    box.z0 = obj->y;
    box.x1 = seg.x0 + obj->x;
    box.z1 = seg.x1 + obj->y;
    for (i = 0; i < WORK->areas->count; i++) {
        AreaTbl *t = WORK->areas;
        e = (Area *)((char *)t + t->offs[i]);
        if (!((e->layers >> obj->layer) & 1)) continue;
        if (!(e->flags & 1)) continue;
        if (e->mask & mode) continue;
        attr = e->attr;
        p = (int *)((char *)e + (e->count * 4 + 8));
        for (j = 0; j < e->count; j++, attr += 4) {
            *(int *)&seg.x0 = *p++;
            *(int *)&seg.x1 = *p;
            if (!(*attr & 1)) continue;
            if (!(*attr & 0x80)) continue;
            if ((*attr & 8) && mode == 0x20) continue;
            if (!func_80032908(&seg, &box)) continue;
            b.vy = 0;
            a.vy = 0;
            a.vx = box.x0 - seg.x0;
            a.vz = box.z0 - seg.z0;
            b.vx = seg.x1 - seg.x0;
            b.vz = seg.z1 - seg.z0;
            func_8007AFA4(&a, &b, &b);
            if (b.vy < 0) continue;
            if (mode != 0x80 || (*(unsigned short *)attr & 0x44)) {
                obj->hit = *attr;
                return 1;
            }
        }
    }
    return 0;
}
