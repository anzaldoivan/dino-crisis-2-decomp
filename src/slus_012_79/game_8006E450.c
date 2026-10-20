#include "common.h"

#include "dc2.h"
typedef Rect0x8006c2d8 Rect0x8006e450;
extern short D_8008F8AC;
extern int func_8007B6EC(Rect0x8006e450 *r, int x, int y);

void func_8006E450(void) {
    Rect0x8006e450 r;

    r.x = 0x1E0;
    r.y = 0x100;
    r.w = 0x20;
    r.h = 0x100 - D_8008F8AC;
    func_8007B6EC(&r, 0x1C0, D_8008F8AC + 0x100);
    r.x = 0x1E0;
    r.w = 0x20;
    r.y = 0x200 - D_8008F8AC;
    r.h = D_8008F8AC;
    func_8007B6EC(&r, 0x1C0, 0x100);
    D_8008F8AC++;
    if (D_8008F8AC >= 0x100) {
        D_8008F8AC = 0;
    }
}
