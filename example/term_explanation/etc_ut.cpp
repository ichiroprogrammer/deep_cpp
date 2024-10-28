#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_GCC_SEQ_POINT;
SUPPRESS_WARN_CLANG_UNSEQUENCED;
int func1() { return 2; }
int func2() { return 1; }

int b1, b2, b3;

void a(int, int, int) {}

TEST(ExpTerm, etc)
{
#ifndef __clang_analyzer__
    int x = 10;

    // @@@ sample begin 0:0

    int i = 0;
    int y = (i = 1) * x + (i = 2);

    a(b1, b2, b3);  // b1, b2, b3の評価順序は規定外
    // @@@ sample end
    IGNORE_UNUSED_VAR(y, i);

    // @@@ sample begin 0:1

    int a      = 1;
    int b      = 2;
    int result = (a < b) ? func1() : func2();
    // @@@ sample end
    IGNORE_UNUSED_VAR(result);
#endif
}
SUPPRESS_WARN_END;
}  // namespace
