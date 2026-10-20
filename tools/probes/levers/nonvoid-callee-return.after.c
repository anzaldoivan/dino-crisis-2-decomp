/* nonvoid-callee-return after (T10.c30, our C): the callee is declared int; its unused return keeps v0 live. */
typedef struct {
    unsigned char pad0[0x54];
    unsigned char state;
    unsigned char pad55[0x6E - 0x55];
    unsigned short t6E;
    unsigned char pad70[0x9E - 0x70];
    short t9E;
    unsigned char padA0[0xA2 - 0xA0];
    unsigned short tA2;
} O;
int func_80048274(O *p);

void func_8002E54C(O *p)
{
    func_80048274(p);
    if (--p->t9E == 0) {
        p->tA2 = p->t6E << 4;
        p->state++;
    }
}
