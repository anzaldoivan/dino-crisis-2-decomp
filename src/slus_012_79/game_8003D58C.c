#include "common.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short x0, z0, x1, z1; } Seg;
typedef struct {
    unsigned char count;
    unsigned char pad[7];
    int offs[1];
} Zones;
typedef struct {
    char pad0[0x4E0];
    void *player;
    char pad4E4[0x5EC - 0x4E4];
    Zones *zones;
} Scratch;
typedef struct {
    char pad0[0x40];
    short x;
    short y;
    short z;
    short pad46;
    short rx;
    short pad4A;
    short rz;
    short pad4E;
    int flags;
    char pad54[0x5A - 0x54];
    short level;
    char pad5C[0x11A - 0x5C];
    unsigned char hit;
} Obj;
int func_80032908(Seg *a, Seg *b);
short func_80033F10(SVECTOR *a, SVECTOR *b, unsigned char c, int d);
void func_80079704(MATRIX *m, SVECTOR *in, SVECTOR *out);
void func_8007A824(SVECTOR *r, MATRIX *m);
void func_8007AFA4(VECTOR *a, VECTOR *b, VECTOR *c);

int func_8003D58C(Obj *obj, int dist, short len, short ang)
{
    SVECTOR out;
    Seg s;
    Seg w;
    Seg seg[2];
    SVECTOR unk38;
    SVECTOR rot;
    MATRIX mat;
    short res[2];
    VECTOR a1;
    VECTOR b1;
    VECTOR a2;
    VECTOR b2;
    unsigned char *p;
    int mask;
    int level;
    Zones *z;
    unsigned char *e;
    int *v;
    unsigned int i;
    unsigned int j;
    unsigned int k;

    p = 0;
    mask = 0x40;
    obj->hit = 0;
    if (obj == (*(Scratch **)0x1F800000)->player) {
        mask = 0x80;
    }
    if (obj->flags & 0x1000) {
        mask = 0x20;
    }
    for (k = 0; k < 2; k++) {
        res[k] = 0;
    }

    rot.vx = obj->rx;
    rot.vy = ang;
    rot.vz = obj->rz;
    func_8007A824(&rot, &mat);
    rot.vx = 0;
    rot.vy = 0;
    rot.vz = len + dist;
    func_80079704(&mat, &rot, &out);

    rot.vx = obj->rx;
    rot.vy = ang + 0x400;
    rot.vz = obj->rz;
    func_8007A824(&rot, &mat);
    rot.vx = 0;
    rot.vy = 0;
    rot.vz = len;
    func_80079704(&mat, &rot, &rot);
    seg[0].x0 = obj->x + rot.vx;
    seg[0].z0 = obj->z + rot.vz;
    seg[0].x1 = out.vx + seg[0].x0;
    seg[0].z1 = out.vz + seg[0].z0;

    rot.vx = obj->rx;
    rot.vy = ang - 0x400;
    rot.vz = obj->rz;
    func_8007A824(&rot, &mat);
    rot.vx = 0;
    rot.vy = 0;
    rot.vz = len;
    func_80079704(&mat, &rot, &rot);
    seg[1].x0 = obj->x + rot.vx;
    seg[1].z0 = obj->z + rot.vz;
    seg[1].x1 = out.vx + seg[1].x0;
    seg[1].z1 = out.vz + seg[1].z0;

    for (i = 0; i < (*(Scratch **)0x1F800000)->zones->count; i++) {
        z = (*(Scratch **)0x1F800000)->zones;
        e = (unsigned char *)z + z->offs[i];
        if (!(e[0] & 1)) continue;
        if (!((*(unsigned short *)(e + 4) >> obj->level) & 1)) continue;
        if (e[3] & mask) continue;
        p = e + 8;
        v = (int *)(e + 8 + e[1] * 4);
        for (j = 0; j < e[1]; j++, p += 4) {
            *(int *)&w.x0 = *v++;
            *(int *)&w.x1 = *v;
            if (!(p[0] & 1)) continue;
            if (!(p[0] & 0x80)) continue;
            for (k = 0; k < 2; k++) {
                s.x0 = seg[k].x0;
                s.z0 = seg[k].z0;
                s.x1 = seg[k].x1;
                s.z1 = seg[k].z1;
                if (!func_80032908(&w, &s)) break;
                b1.vy = 0;
                a1.vy = 0;
                a1.vx = s.x0 - w.x0;
                a1.vz = s.z0 - w.z0;
                b1.vx = w.x1 - w.x0;
                b1.vz = w.z1 - w.z0;
                func_8007AFA4(&a1, &b1, &b1);
                if (b1.vy < 0) break;
                if (mask == 0x80 && !(*(unsigned short *)p & 0x44)) break;
                if (k == 1) {
                    obj->hit = p[0];
                    goto done;
                }
            }
        }
    }
done:
    if (obj->hit == 0) {
        return 0;
    }
    if (p[0] & 0x40) {
        level = obj->level - 2;
    } else {
        level = obj->level + 2;
    }

    for (i = 0; i < (*(Scratch **)0x1F800000)->zones->count; i++) {
        z = (*(Scratch **)0x1F800000)->zones;
        e = (unsigned char *)z + z->offs[i];
        if (!(e[0] & 1)) continue;
        if (!((*(unsigned short *)(e + 4) >> level) & 1)) continue;
        if (e[3] & mask) continue;
        p = e + 8;
        v = (int *)(e + 8 + e[1] * 4);
        for (j = 0; j < e[1]; j++, p += 4) {
            *(int *)&w.x0 = *v++;
            *(int *)&w.x1 = *v;
            if (!(p[0] & 1)) continue;
            if (p[0] & 0x80) continue;
            for (k = 0; k < 2; k++) {
                s.x0 = seg[k].x0;
                s.z0 = seg[k].z0;
                s.x1 = seg[k].x1;
                s.z1 = seg[k].z1;
                if (!func_80032908(&w, &s)) continue;
                b2.vy = 0;
                a2.vy = 0;
                a2.vx = s.x0 - w.x0;
                a2.vz = s.z0 - w.z0;
                b2.vx = w.x1 - w.x0;
                b2.vz = w.z1 - w.z0;
                func_8007AFA4(&a2, &b2, &b2);
                if (b2.vy >= 0) return 0;
            }
        }
    }

    for (k = 0; k < 2; k++) {
        rot.vx = seg[k].x1;
        rot.vy = -(level * 1000);
        rot.vz = seg[k].z1;
        res[k] = func_80033F10(&rot, &rot, mask, 0);
    }
    if (res[0] == res[1] && res[0] != obj->y) {
        return 1;
    }
    return 0;
}
