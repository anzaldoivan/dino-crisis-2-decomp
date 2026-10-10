/* tools/probes/selftest.c -- our own probe body for `tools/probe.py --selftest` (no game data).
 * The loop makes -O1 and -O2 emit different code (strength reduction / scheduling): the known-false control. */
int probe_selftest(int *a, int n, int k)
{
    int i, s = 0;

    for (i = 0; i < n; i++) {
        s += a[i] * k + (a[i + 1] >> 2);
    }
    return s;
}
