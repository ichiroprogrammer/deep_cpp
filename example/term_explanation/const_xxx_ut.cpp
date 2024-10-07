#include <string>
#include <type_traits>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {

TEST(TermExp, just_const)
{
    // @@@ sample begin 0:0

    using namespace std;
    auto const str = string{"str"};  // strはプログラムがこの行を通過するときに初期化

    char const* c_str = str.c_str();
    IGNORE_UNUSED_VAR(str, c_str);  // @@@ delete

    static_assert(!is_const_v<decltype(c_str)>);
    c_str = nullptr;  // c_strは変数としてconstではない
    static_assert(is_const_v<remove_reference_t<decltype(*c_str)>>);  // *cは_strはconst
    static_assert(is_same_v<char const&, decltype(*c_str)>);  // *c_strはconstリファレンス

    char const* const cc_str = c_str;

    static_assert(is_const_v<decltype(cc_str)>);
    // cc_str = nullptr;  // cc_strは変数としてconstであるためコンパイルエラー
    static_assert(is_const_v<remove_reference_t<decltype(*cc_str)>>);  // *cc_strはconst
    static_assert(is_same_v<char const&, decltype(*cc_str)>);  // *cc_strはconstリファレンス

    constexpr int c_int = 1;
    static_assert(is_const_v<decltype(c_int)>);  // c_intはcons
    // @@@ sample end

    IGNORE_UNUSED_VAR(str, cc_str, c_int);
}

// @@@ sample begin 1:0

template <int N>
struct Templ {
    static constexpr auto value = N;  // valueは定数
};
// @@@ sample end
TEST(TermExp, constexpr_constant)
{
    // @@@ sample begin 1:1

    constexpr int a = 5;  // aは定数であるためかきのような使い方ができる
    static_assert(a == 5);

    constexpr int b = 5;  // bは定数でないため、下記のような使い方ができない
    // static_assert(b == 5);  // コンパイルエラー

    constexpr double PI{3.14159265358979323846};  // PIはconstexpr

    auto templ = Templ<a>{};  // aはconstexprなのでaの初期化が可能

    static_assert(templ.value == 5);
    // @@@ sample end

    IGNORE_UNUSED_VAR(a, b, PI);
}

// clang-format off
// @@@ sample begin 2:0

constexpr int f(int a) noexcept { return a * 3; }  // aがconstexprならばf(a)もconstexpr
int g(int a) noexcept { return a * 3; }            // aがconstexprであってもg(a)は非constexpr
// @@@ sample end
// clang-format on

TEST(TermExp, constexpr_func)
{
    // clang-format off
    // @@@ sample begin 2:1

    auto x = int{0};

    constexpr auto a = f(3);     // f(3)はconstexprなのでaの初期化が可能
    // constexpr auto b = f(x);  // xは非constexprなのでbの初期化はコンパイルエラー
    auto const c = f(3);         // cはconstexpr定数と定義とすべき
    // constexpr auto d = g(3);  // g(3)は非constexprなのでdの初期化はコンパイルエラー
    auto const e = g(x);         // eはここで初期化して、この後不変
    // @@@ sample end
    // clang-format on

    IGNORE_UNUSED_VAR(x, a, c, e);
}

// clang-format off
// @@@ sample begin 3:0

class Integer {
public:
    constexpr Integer(int32_t integer) noexcept : integer_{integer} {}
    constexpr operator int() const noexcept { return integer_; }  // constexprメンバ関数はconst
    constexpr int32_t Allways2() const noexcept { return 2; }     // constexprメンバ関数はconst
    static constexpr int32_t Allways3() noexcept { return 3; }    // static関数のconstexpr化

private:
    int32_t integer_;
};
// @@@ sample end
// clang-format on

TEST(TermExp, constexpr_instance)
{
    // clang-format off
    // @@@ sample begin 3:1

    constexpr auto i5 = 5;                // i5はconstexprインスタンス
    constexpr auto int_5 = Integer{i5};   // int_5はconstexprインスタンス
    static_assert(int_5 == 5);            // intへの暗黙の型変換

    auto i3  = 3;                         // i3はconstexpr定数ではない
    auto int_3 = Integer{i3};             // int_3はconstexprインスタンスではない
    // static_assert(int_3 == 5);         // int_3がconstexprではないため、コンパイルエラー
    static_assert(int_3.Allways2() == 2); // int_3はconstexprインスタンスではないが、
                                          // int_3.Allways2()はconstexprt定数
    static_assert(int_3.Allways3() == 3); // int_3はconstexprインスタンスではないが、
                                          // int_3.Allways3()はconstexprt定数
    // @@@ sample end
    // clang-format on
    IGNORE_UNUSED_VAR(i3);
}

// @@@ sample begin 4:0

constexpr Integer operator"" _i(unsigned long long int value)  // ユーザ定義リテラルの定義
{
    return Integer(static_cast<int32_t>(value));
}
// @@@ sample end

TEST(TermExp, user_defined_literal)
{
    // @@@ sample begin 4:1

    constexpr auto i = 123_i;
    static_assert(i == 123);
    static_assert(std::is_same_v<decltype(i), Integer const>);
    // @@@ sample end
}
}  // namespace
