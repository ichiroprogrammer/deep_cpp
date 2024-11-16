#include <tuple>
#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
namespace one_argue_constructorwith_explicit {
// @@@ sample begin 0:0

struct A {
    A(int a) : x{a} {}
    int x;
};

A f(A a) { return a; };
// @@@ sample end
TEST(ExpTerm, explicit)
{
    // @@@ sample begin 0:1

    A a = 1;  // A::Aがexplicitでないため、iはA{1}に変換される
    ASSERT_EQ(a.x, 1);

    auto b = f(2);  // A::Aがexplicitでないため、2はA{2}に変換される
    ASSERT_EQ(b.x, 2);
    // @@@ sample end
}
}  // namespace one_argue_constructorwith_explicit

namespace one_argue_constructor_without_explicit {

// @@@ sample begin 1:0

struct A {
    explicit A(int a) : x{a} {}  // 暗黙の型変換の抑止
    int x;
};

A f(A a) { return a; };
// @@@ sample end
TEST(ExpTerm, explicit)
{
    // @@@ sample begin 1:1

    // A a = 1;    // A::Aがexplicitであるため、コンパイルエラー
    // auto b = f(2);  // A::Aがexplicitであるため、コンパイルエラー
    // @@@ sample end

    f(A{1});
}
}  // namespace one_argue_constructor_without_explicit

namespace any_argue_constructor_without_explicit {
// @@@ sample begin 2:0

struct A {
    A(int a, int b) : x{a}, y{b} {}
    int x;
    int y;
};

A    f(A a) { return a; };
bool operator==(A lhs, A rhs) { return std::tuple(lhs.x, lhs.x) == std::tuple(rhs.x, rhs.x); }
// @@@ sample end

TEST(ExpTerm, explicit)
{
    // @@@ sample begin 2:1

    A a = {1, 2};  // A::Aがexplicitでないため、iはA{1, 2}に変換される
    ASSERT_EQ(a, (A{1, 2}));

    auto b = f({2, 1});  // A::Aがexplicitでないため、2はA{2,1}に変換される
    ASSERT_EQ(b, (A{2, 1}));
    // @@@ sample end
}
}  // namespace any_argue_constructor_without_explicit

namespace any_argue_constructor_with_explicit {
// @@@ sample begin 3:0

struct A {
    explicit A(int a, int b) : x{a}, y{b} {}
    int x;
    int y;
};

A    f(A a) { return a; };
bool operator==(A lhs, A rhs) { return std::tuple(lhs.x, lhs.x) == std::tuple(rhs.x, rhs.x); }
// @@@ sample end

TEST(ExpTerm, explicit)
{
    // @@@ sample begin 3:1

    // A a = {1, 2};  // A::Aがexplicitであるため、コンパイルエラー
    // auto b = f({2, 1});  // A::Aがexplicitであるため、コンパイルエラー
    // @@@ sample end

    auto a = A{1, 2};
    auto b = f(a);

    ASSERT_EQ(a, b);
}
}  // namespace any_argue_constructor_with_explicit

namespace operator_bool {
// @@@ sample begin 4:0

struct A {
    // clang-format off
    explicit A(int a) : x{a} {}  // 暗黙の型変換の抑止
    operator bool() const noexcept { return x; }
    // clang-format on
    int x;
};
// @@@ sample end

TEST(ExpTerm, explicit)
{
    // @@@ sample begin 4:1

    auto a = A{2};

    ASSERT_TRUE(a);
    ASSERT_EQ(1, a);  // aをboolに変換するとtrue、trueをintに変換すると1

    int b = a + 1;  // aをboolに変換するとtrue、trueをintに変換すると1であるため、bは2
    ASSERT_EQ(b, 2);

    // @@@ sample end
}
}  // namespace operator_bool
namespace operator_bool_explicit {
// @@@ sample begin 5:0

struct A {
    // clang-format off
    explicit A(int a) : x{a} {}  // 暗黙の型変換の抑止
    explicit operator bool() const noexcept { return x; }// 暗黙の型変換の抑止
    // clang-format on
    int x;
};
// @@@ sample end

TEST(ExpTerm, explicit)
{
    // @@@ sample begin 5:1

    auto a = A{2};

    // ASSERT_EQ(1, a);  // operator boolがexplicitであるため、コンパイルエラー
    // int b = a + 1;  // operator boolがexplicitであるため、コンパイルエラー
    // @@@ sample end
    IGNORE_UNUSED_VAR(a);
}
}  // namespace operator_bool_explicit

// clang-format off
// @@@ sample begin 6:0

template <typename T>  // Tが整数型の場合、暗黙の型変換を許可
struct S {
#if __cplusplus >= 202002L  // C++20

    explicit(!std::is_integral_v<T>) S(T x) : value{x} {}
#else  // C++17

    // T が整数型でない場合に有効なコンストラクタ
    template <typename U = T, std::enable_if_t<!std::is_integral_v<U>>* = nullptr>
    explicit S(U x) : value{x} { }

    // T が整数型の場合に有効な非explicitコンストラクタ
    // T が整数型の場合に有効な非explicitコンストラクタ
    template <typename U = T, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
    S(U x) : value{x} { }
#endif

    T value;
};

template <typename T>  // 推論ガイド
S(T)->S<T>;
// @@@ sample end
// clang-format on
TEST(ExpTerm, explicit_cond)
{
    // clang-format off
    // @@@ sample begin 6:1

    S s = 1;      // Tがintであるため、explicit宣言されていないため、暗黙の型変換は許可
    // S t = 1.0; // Tが整数型でないため暗黙の型変換は禁止であるため、コンパイルエラー
    S t{1.0};     // Tが整数型でないが、明示的な初期化は問題ない

    ASSERT_EQ(s.value, 1);
    // @@@ sample end
    // clang-format on
    IGNORE_UNUSED_VAR(t);
}

// clang-format off
// @@@ sample begin 7:0

template <typename T>
struct Optional {
#if __cplusplus >= 202002L  // C++20
    explicit(std::is_same_v<T, std::nullptr_t>) Optional(const T& value)
        : has_value_(!std::is_same_v<T, std::nullptr_t>), value_(value) { }

#else  // C++17

    // Tがnullptr_tではない場合に有効なコンストラクタ
    template <typename U = T, std::enable_if_t<!std::is_same_v<U, std::nullptr_t>>* = nullptr>
    Optional(const U& value) : has_value_(true), value_(value) { }

    // Tがnullptr_tの場合に有効なexplicitコンストラクタ
    template <typename U = T, std::enable_if_t<std::is_same_v<U, std::nullptr_t>>* = nullptr>
    explicit Optional(const U& value) : has_value_(false), value_(value) { }
#endif

    explicit operator bool() const noexcept { return has_value_; }  // bool型への変換
             operator T() const noexcept { return value_; }         // T型への変換

private:
    bool has_value_;
    T    value_;
};
template <typename T>  // 推論ガイド
Optional(T)->Optional<T>;
// @@@ sample end
// clang-format on

TEST(ExpTerm, explicit_cond2)
{
    // @@@ sample begin 7:1

    Optional a = 2;   // T == intであるため、暗黙の型変換を許可
    ASSERT_TRUE(a);   // has_value_がtrueであるため
    ASSERT_EQ(a, 2);  // T型への暗黙的変換をチェック

    // Optional n = nullptr; // T == std::nullptr_tのため暗黙の型変換抑止により、コンパイルエラー
    Optional n{nullptr};  // 通常の初期化
    ASSERT_FALSE(n);
    // @@@ sample end
}

}  // namespace
