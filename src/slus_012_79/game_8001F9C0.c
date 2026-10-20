#include "common.h"

void func_8001F9C0(unsigned char *src, unsigned char *dst, int size)
{
    unsigned int i;
    int bits;
    unsigned int flags;
    unsigned int lo;
    unsigned int hi;
    unsigned int off;
    unsigned int len;
    unsigned char *p;

    bits = 0;
    flags = 0;
    do {
        if (bits == 0) {
            flags = *src++;
            bits = 8;
            size--;
        }
        bits--;
        size--;
        if (flags & 1) {
            *dst++ = *src++;
            flags >>= 1;
        } else {
            size--;
            lo = *src++;
            hi = *src++;
            i = 0;
            flags >>= 1;
            off = ((hi & 0xF) << 8) | lo;
            len = (hi >> 4) + 2;
            p = dst - off;
            if (len != 0) {
                do {
                    *dst++ = *p++;
                } while (++i < len);
            }
        }
    } while (size > 0);
}
