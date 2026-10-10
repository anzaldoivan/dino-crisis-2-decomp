/* func_8005A34C.h -- unit-local types of the shared body of family slus_012_79:0x8005a34c (T7.c27), renamed <T>_s8005A34C so
 * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */
#ifndef SHARED_H_8005A34C
#define SHARED_H_8005A34C

typedef struct {
    unsigned char *p0;
    unsigned char *p4;
    unsigned char b8;
    unsigned char b9;
    unsigned char pad[2];
} Ent0C_s8005A34C;
typedef struct {
    int h0;
    Ent0C_s8005A34C e[1];
} Tbl_s8005A34C;

#endif
