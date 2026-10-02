/* func_8002E920.h -- unit-local types of the shared body of family slus_012_79:0x8002e920 (T7.c27), renamed <T>_s8002E920 so
 * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */
#ifndef SHARED_H_8002E920
#define SHARED_H_8002E920

typedef struct {
    char pad0[0x40];
    short x;                    
    short y;                    
    short z;                    
    char pad46[0x54 - 0x46];
    unsigned char state;           
    char pad55[0x7C - 0x55];
    short vx;                   
    short vy;                   
    short vz;                   
    char pad82[0x84 - 0x82];
    unsigned char flags;           
} Obj8002E920_s8002E920;

#endif
