#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace undefined_behavior {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_UNUSED_VAR;
SUPPRESS_WARN_GCC_SEQ_POINT;
SUPPRESS_WARN_CLANG_UNSEQUENCED;
int index;
TEST(ExpTerm, DISABLED_undefined_behavior)
{
#ifndef __clang_analyzer__
    // @@@ sample begin 0:0

    int a = 42;
    int b = 0;
    int c = a / b;  // 未定義動作 - ゼロ除算

    int arr[]{1, 2, 3};
    int x = arr[index];  // 未定義動作 - index>2の場合、配列範囲外アクセス

    // @@@ sample end
    IGNORE_UNUSED_VAR(c, arr, x);
#endif
}
}  // namespace undefined_behavior

namespace unspecified_behavior {

int func(int x) { return x * 2; }

TEST(ExpTerm, unspecified_behavior)
{
    // @@@ sample begin 1:0

    enum class MyEnum : int { Value1 = 1, Value2 = 256 };
    int value = static_cast<int8_t>(MyEnum::Value2);  // 未規定 - 256はint8_tとして表現できない

    auto a      = int{5};
    auto lambda = [](auto a0, auto a1) { return a0 / a1; };
    auto result = lambda(a++, a++);  // 未規定 - 引数評価の順序が決まっていない
    // @@@ sample end
    IGNORE_UNUSED_VAR(result);
}

SUPPRESS_WARN_END;
}  // namespace unspecified_behavior
