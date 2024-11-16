#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

template <typename T>  // Tが整数型の場合、暗黙の型変換を許可
struct S {
    // T が整数型でない場合に有効なコンストラクタ
    template <typename U = T, std::enable_if_t<!std::is_integral_v<U>>* = nullptr>
    explicit S(U x) : value{x}
    {
    }

    // T が整数型の場合に有効な非explicitコンストラクタ
    template <typename U = T, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
    S(U x) : value{x}
    {
    }

    T value;
};

// @@@ sample end

TEST(Template, deduction_guide)
{
    // @@@ sample begin 0:1

    S<int>    s1{42};   // 明示的にテンプレート引数を指定
    S<double> s2{1.0};  // 明示的にテンプレート引数を指定

    //  clang-format off
    #if 0  // テンプレート引数の推論ができず、下記はコンパイルできない 
    S       s1{42};   // 明示的にテンプレート引数を指定
    S       s2{1.0};  // 明示的にテンプレート引数を指定
    #endif
    // @@@ sample end
    //  clang-format on

    IGNORE_UNUSED_VAR(s1, s2);
}

// @@@ sample begin 0:2

template <typename T>
S(T) -> S<T>;
// @@@ sample end

TEST(Template, deduction_guide2)
{
    // @@@ sample begin 0:3

    S s1{42};   // 推論ガイドの効果
    S s2{1.0};  // 推論ガイドの効果
    S s3 = 42;  // S<int>のコンストラクタがintであるため、暗黙の型変換が可能
    // S    s4 = 1.0;  // S<double>のコンストラクタがexplicitであるため
    // @@@ sample end

    IGNORE_UNUSED_VAR(s1, s2, s3);
}
}  // namespace
