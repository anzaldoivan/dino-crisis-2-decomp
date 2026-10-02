/* func_800661FC.h -- unit-local types of the shared body of family slus_012_79:0x800661fc (T7.c27), renamed <T>_s800661FC so
 * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */
#ifndef SHARED_H_800661FC
#define SHARED_H_800661FC

typedef struct {
    unsigned char pad0[0x1C];
    unsigned char r0, g0, b0, p0;
    unsigned char r1, g1, b1, p1;
    unsigned char r2, g2, b2, p2;
} A28_s800661FC;
typedef struct {
    unsigned char pad0[0x24];
    unsigned char r0, g0, b0, p0;
    unsigned char r1, g1, b1, p1;
    unsigned char r2, g2, b2, p2;
    unsigned char r3, g3, b3, p3;
} A34_s800661FC;
typedef struct {
    A28_s800661FC *a;
    A34_s800661FC *b;
    unsigned short na;
    unsigned short nb;
} Hdr_s800661FC;
typedef struct {
    char pad[0x68];
    Hdr_s800661FC *hdr;
} Obj_s800661FC;

#endif
