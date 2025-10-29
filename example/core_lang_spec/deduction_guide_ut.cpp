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

    // 型推論ガイドがないため、下記はコンパイルできない
    // S s1{42};   // エラー: テンプレート引数を推論できない
    // S s2{1.0};  // エラー: テンプレート引数を推論できない

    // 上記の問題を回避するためには型推論ガイドを定義するか、テンプレート引数を指定しなければならない
    S<int>    s1{42};   // 明示的にテンプレート引数を指定
    S<double> s2{1.0};  // 明示的にテンプレート引数を指定
    // @@@ sample end
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
    S s3 = 42;  // OK: S<int>のコンストラクタが非explicitのため、暗黙の変換が可能
    // S    s4 = 1.0;  // S<double>のコンストラクタがexplicitであるため、暗黙の変換不可
    // @@@ sample end

    IGNORE_UNUSED_VAR(s1, s2, s3);
}
}  // namespace
