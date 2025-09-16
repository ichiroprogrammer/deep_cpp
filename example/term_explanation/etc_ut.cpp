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
TEST(ExpTerm, narrowing)
{
    // @@@ sample begin 1:0

    int32_t large  = 300;
    int8_t  small  = large;  // 縮小型変換
    bool    b      = large;
    double  d      = large;  // 単単なる型変換(縮小ではない)
    int32_t large2 = d;      // 縮小型変換

    // large = int32_t{d};   縮小型変換回避のためリスト初期化の使用。コンパイルエラー
    // @@@ sample end
    IGNORE_UNUSED_VAR(large, large2, b, d, small);
}
}  // namespace

void func();
namespace A {
void func() {}
}  // namespace A

TEST(ExpTerm, qualified_call)
{
    // @@@ sample begin 2:0

    extern void func();  // グローバル名前空間での宣言

    struct Base {
        void func() const noexcept {}
    };

    A::func();  // 名前空間名による修飾

    struct Derived : Base {
        // void        func() { func(); /* funcの無限リカーシブコール */ }
        void        func() { Base::func(); /* クラス名での修飾 */ }
        void        func(int) { ::func(); /* グローバル修飾 */ }
        void        func(Base) { this->func(); /* thisによる修飾 */ }
        static void func(std::string) {}
    };

    Base b;
    b.func();        // 通常の関数呼び出し
    b.Base::func();  // クラス名での修飾による関数呼び出し

    Derived d;
    Derived::func("str");  // クラス名での修飾による関数呼び出し
    d.func("str");         // 通常の関数呼び出し
    // @@@ sample end
}
