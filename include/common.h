/* common.h — fixed-width types and INCLUDE_ASM for every C unit (cc1 psyq4.6: int/long 32-bit, short 16-bit). */
#ifndef COMMON_H
#define COMMON_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#ifndef NULL
#define NULL ((void *)0)
#endif

#include "include_asm.h"

#endif
