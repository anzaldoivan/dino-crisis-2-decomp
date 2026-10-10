/* func_8002D78C.h -- unit-local types of the shared body of family slus_012_79:0x8002d78c (T7.c27), renamed <T>_s8002D78C so
 * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */
#ifndef SHARED_H_8002D78C
#define SHARED_H_8002D78C

typedef struct Obj_s8002D78C {
    unsigned char pad0[0x46];
    unsigned char timer;                
    unsigned char pad47[0x54 - 0x47];
    unsigned char mode;                 
    unsigned char pad55[0x7B - 0x55];
    unsigned char state;                
} Obj_s8002D78C;

#endif
