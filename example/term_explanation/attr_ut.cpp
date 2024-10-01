
#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace attr_NS {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_DEPRECATED_DECL;

// @@@ sample begin 0:0

// 非推奨の関数
[[deprecated("この関数は非推奨です。代わりに newFunction を使用してください。")]]  // 
// clang-format off
void oldFunction();  // この関数を呼び出すと警告される
void newFunction();
// @@@ sample end
// clang-format on

// @@@ sample begin 1:0
void processValues()
{
    [[maybe_unused]] int unusedValue = 42;  // 使用しない変数でも警告が出ない

    // do something
}
// @@@ sample end
// @@@ sample begin 2:0

[[nodiscard]] int computeResult() { return 42; }
// @@@ sample end

TEST(cpp20, attr)
{
    SUPPRESS_WARN_GCC_BEGIN;
    _Pragma("GCC diagnostic ignored \"-Wunused-result\"");

    // @@@ sample begin 2:1

    computeResult();               // 警告が出る：戻り値が無視されている
    int result = computeResult();  // これはOK
    // @@@ sample end

    IGNORE_UNUSED_VAR(result);

    SUPPRESS_WARN_GCC_END;
}
SUPPRESS_WARN_END;
void oldFunction() {}
void newFunction() {}

void checkValue(int value)
{
    // @@@ sample begin 3:0

    switch (value) {
    case 1:
        // do something
        [[fallthrough]];  // 明示的に fallthrough を宣言することができる
    case 2:
        // do something
        break;
    default:
        break;
    }
    // @@@ sample end
}

// @@@ sample begin 4:0

struct Empty {};

struct Data {
    int                         value;
    [[no_unique_address]] Empty e;  // メモリのパディングを省略して効率化
};
// @@@ sample end
}  // namespace attr_NS
