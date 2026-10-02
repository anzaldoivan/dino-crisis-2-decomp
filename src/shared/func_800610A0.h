/* func_800610A0.h -- unit-local types of the shared body of family slus_012_79:0x800610a0 (T7.c27), renamed <T>_s800610A0 so
 * a member unit's own types never collide; guarded: a unit may instantiate the family twice. */
#ifndef SHARED_H_800610A0
#define SHARED_H_800610A0

typedef struct {
    int pc;
    char pad04[0x80];
    int *arg;
} ScriptCtx_s800610A0;

#endif
