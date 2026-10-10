/* func_800631D8.h -- unit-local types of the shared body of family slus_012_79:0x800631d8 (T7.c27), renamed <T>_s800631D8 so
 * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */
#ifndef SHARED_H_800631D8
#define SHARED_H_800631D8

typedef struct {
    int pc;
    char pad04[0x80];
    int *arg;
} ScriptCtx_s800631D8;

#endif
