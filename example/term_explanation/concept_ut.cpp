#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>

#include "gtest_wrapper.h"

namespace {

namespace old_style {
// @@@ sample begin 0:0

// SFINAEを使用したC++17スタイル
template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
T add(T a, T b)
{
    return a + b;
}
// @@@ sample end

TEST(cpp20, concept)
{
    // @@@ sample begin 0:1

    ASSERT_EQ(add(10, 20), 30);     // int型
    ASSERT_EQ(add(1.5, 2.5), 4.0);  // double型

    auto str1 = std::string{"Hello, "};
    auto str2 = std::string{"World!"};
    // add(str1, str2);  // これを試すとコンパイルエラー
    // concept_ut.cpp:10:3: note: candidate: ‘template<class T, class> T
    // {anonymous}::old_style::add(T, T)’
    //    10 | T add(T a, T b) {
    //       |   ^~~
    // concept_ut.cpp:10:3: note:   template argument deduction/substitution failed:
    // concept_ut.cpp:9:22: error: no type named ‘type’ in ‘struct std::enable_if<false, void>’
    //     9 | template<typename T, typename = typename
    //     std::enable_if<std::is_arithmetic<T>::value>::type>
    //       |                      ^~~~~~~~
    // エラーメッセージがわかりずらい
    // @@@ sample end
}
}  // namespace old_style

namespace new_style {

// @@@ sample begin 1:0

// コンセプトを使用したC++20スタイル
template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <Arithmetic T>
T add(T a, T b)
{
    return a + b;
}
// @@@ sample end

TEST(cpp20, concept)
{
    // @@@ sample begin 1:1

    ASSERT_EQ(add(10, 20), 30);     // int型
    ASSERT_EQ(add(1.5, 2.5), 4.0);  // double型

    auto str1 = std::string{"Hello, "};
    auto str2 = std::string{"World!"};
    // add(str1, str2);  // これを試すとコンパイルエラー
    // concept_ut.cpp:36:27: note: the expression ‘is_arithmetic_v<T> [with T =
    // std::basic_string<char, std
    // ::char_traits<char>, std::allocator<char> >]’ evaluated to ‘false’
    //    36 | concept Arithmetic = std::is_arithmetic_v<T>;
    //       |                      ~~~~~^~~~~~~~~~~~~~~~~~
    // ↑  エラーメッセージがわかりよい。テンプレートTがコンセプトfalseとなる
    // @@@ sample end
}
}  // namespace new_style

namespace old_style {
// @@@ sample begin 2:0

// 制約のためにstatic_assertを使用したC++17スタイル
template <typename FLOAT_0, typename FLOAT_1>
bool is_equal(FLOAT_0 lhs, FLOAT_1 rhs) noexcept
{
    static_assert(std::is_floating_point_v<FLOAT_0>, "FLOAT_0 shoud be float or double.");
    static_assert(std::is_same_v<FLOAT_0, FLOAT_1>, "FLOAT_0 and FLOAT_1 shoud be a same type.");

    return std::abs(lhs - rhs) <= std::numeric_limits<FLOAT_0>::epsilon();
}
// @@@ sample end

TEST(cpp20, use_static_assert)
{
    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    // a + bはfloatであり、0.05はdoubleであるため、下記コードはコンパイルできない。
    // ASSERT_TRUE(is_equal(0.05, a + b));
    ASSERT_TRUE(is_equal(0.05F, a + b));  // OK リテラルに型を指定して、引数の型を統一
}
}  // namespace old_style

namespace new_style {

// @@@ sample begin 3:0

// 標準コンセプト std::floating_point と std::same_as を使用
template <std::floating_point FLOAT_0, std::same_as<FLOAT_0> FLOAT_1>
bool is_equal(FLOAT_0 lhs, FLOAT_1 rhs) noexcept
{
    return std::abs(lhs - rhs) <= std::numeric_limits<FLOAT_0>::epsilon();
}
// @@@ sample end

TEST(cpp20, use_concept)
{
    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    // a + bはfloatであり、0.05はdoubleであるため、下記コードはコンパイルできない。
    // ASSERT_TRUE(is_equal(0.05, a + b));
    ASSERT_TRUE(is_equal(0.05F, a + b));  // OK リテラルに型を指定して、引数の型を統一
}
}  // namespace new_style

namespace new_style2 {

// @@@ sample begin 4:0

// requiresを使った関数テンプレートの制約
template <typename FLOAT_0, typename FLOAT_1>
requires std::floating_point<FLOAT_0> && std::same_as<FLOAT_0, FLOAT_1>
bool is_equal(FLOAT_0 lhs, FLOAT_1 rhs) noexcept
{
    return std::abs(lhs - rhs) <= std::numeric_limits<FLOAT_0>::epsilon();
}

// @@@ sample end

TEST(cpp20, use_requires)
{
    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    // a + bはfloatであり、0.05はdoubleであるため、下記コードはコンパイルできない。
    // ASSERT_TRUE(is_equal(0.05, a + b));
    ASSERT_TRUE(is_equal(0.05F, a + b));  // OK リテラルに型を指定して、引数の型を統一
}
}  // namespace new_style2
}  // namespace
