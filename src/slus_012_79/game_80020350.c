#include "common.h"

#include "dc2.h"
typedef struct {
    int x0;
    int size;
    unsigned char *dst;
    unsigned char bits;
    unsigned char flags;
    unsigned char state;
    unsigned char lo;
} Lz0x80020350;
extern unsigned char D_800AF629;
extern char *D_80087564[];

void func_80020350(int a0, Lz0x80020350 *s)
{
    unsigned char *dst;
    int n;
    unsigned int flags;
    int n2;
    unsigned int bits;
    unsigned int lo;
    unsigned char *src;
    unsigned int i;
    unsigned char *p;
    unsigned int len;
    unsigned int hi;

    n = 0x800;
    if (s->size < 0x801) {
        n = s->size;
    }
    dst = s->dst;
    bits = s->bits;
    flags = s->flags;
    lo = s->lo;
    src = (unsigned char *)D_80087564[D_800AF629];
    switch (s->state) {
    case 1:
        // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
        goto chk;
    case 2:
        // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
        goto second;
    }
    // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
    goto top;
first:
    lo = *src++;
    if (--n == 0) {
        s->state = 2;
        // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
        goto save;
    }
second:
    /* loop depth on the match block sets the allocation priorities of i, n, n2 (dead loop) */
    for (;;) {
    hi = *src++;
    i = 0;
    n2 = n - 1;
    n = flags >> 1;
    len = (hi >> 4) + 2;
    p = dst - (((hi & 0xF) << 8) | lo);
    if (len != 0) {
        do {
            *dst++ = *p++;
            i++;
        } while (i < len);
    }
    flags = n;
    n = n2;
    // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
    goto next;
    }
top:
    if (bits == 0) {
        flags = *src++;
        n--;
        bits = 8;
        if (n == 0) {
            s->state = 1;
            // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
            goto save;
        }
    }
chk:
    bits--;
    if (!(flags & 1)) {
        // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
        goto first;
    }
    *dst++ = *src++;
    flags >>= 1;
    n--;
next:
    if (n != 0) {
        // !FAKE: resumable decoder state (s->state re-entry); block order needs the label
        goto top;
    }
    s->state = 0;
save:
    s->dst = dst;
    s->bits = bits;
    s->flags = flags;
    s->lo = lo;
}
