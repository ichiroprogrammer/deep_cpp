#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Rvalue {

void f()

{
    // @@@ sample begin 0:0
    // sを初期化するためにstd::string{}により生成されるオブジェクトはprvalue、 sはlvalue
    //   ↓lvalue
    auto s = std::string{};  // この式の左辺はテンポラリオブジェクト(つまりprvalue)

    /*
    auto* sp = &std::string{};
    ↑は、コンパイルエラー
    ↓は、その時のg++のエラーメッセージ
    error: taking address of rvalue
    */

    // 下記のようにすればアドレスを取得できるが、このようなことはすべきではない。
    auto&& rvalue_ref = std::string{};
    auto   sp         = &rvalue_ref;  // spはrvalue_refのアドレスを指しているが、、、
    // @@@ sample end

    static_assert(std::is_same_v<std::string*, decltype(sp)>);
    IGNORE_UNUSED_VAR(s, sp);
}
}  // namespace Rvalue

namespace ref_pattern1 {

TEST(Expression, lvalue_ref0)
{
    // @@@ sample begin 1:0

    int  a     = 0;
    int& a_ref = a;  // a_refはaのリファレンス
                     // a_refはaをバインドする

    ASSERT_EQ(&a, &a_ref);  // リファレンスは別名に過ぎないため、このテストが成立

    int b = 1;
    a_ref = b;  // 一見、a_refの再初期化に見えるが、実際は値の代入になるため、以下のテストが成立
    ASSERT_EQ(a, b);  // リファレンスは別名に過ぎないため、このテストが成立

    /*
    int& t_ref = int(99);  非const lvalueリファレンスはrvalueをバインドできない */
    int const& t_ref = int(99);  // 上記とは異なり、const lvalueリファレンスはrvalueをバインドできる
    ASSERT_EQ(t_ref, 99);
    // @@@ sample end
}

// clang-format off
// @@@ sample begin 1:1

int f(int& )        { return 0; }
int f(int const & ) { return 1; }
// @@@ sample end
// clang-format on

TEST(Expression, lvalue_ref1)
{
    // @@@ sample begin 1:2

    int       a = 0;
    int const b = 0;

    ASSERT_EQ(0, f(a));
    ASSERT_EQ(1, f(b));      // constオブジェクトのバインド
    ASSERT_EQ(1, f(int()));  // rvalueのバインド
    // @@@ sample end
}
}  // namespace ref_pattern1

namespace ref_pattern2 {

TEST(Expression, rvalue_ref1)
{
    // @@@ sample begin 2:0

    int        a      = 0;
    int const& a_ref0 = a;        // const lvalueリファレンス
    int const& a_ref1 = int(99);  // const lvalueリファレンスはrvalueをバインドできる
    int&& a_ref2 = int(99);  // rvalueリファレンスはテンポラリオブジェクトをバインドできる

    ASSERT_EQ(a_ref1, 99);
    ASSERT_EQ(a_ref2, 99);
    // @@@ sample end
    ASSERT_EQ(a_ref0, 0);
}
// clang-format off
// @@@ sample begin 2:1

int f(std::string&)       { return 0; } // f-0
int f(std::string const&) { return 1; } // f-1
int f(std::string&&)      { return 2; } // f-2
// @@@ sample end
// clang-format on

TEST(Expression, rvalue_ref2)
{
    // clang-format off
    // @@@ sample begin 2:2

    std::string       str;
    std::string const cstr;

    ASSERT_EQ(0, f(str));            // f-0の呼び出し
    ASSERT_EQ(1, f(cstr));           // f-1の呼び出し、const lvalueリファレンスのバインド
    ASSERT_EQ(2, f(std::string{}));  // f-2の呼び出し、f-2が無ければ、f-1を呼び出すが
    ASSERT_EQ(2, f(std::move(str))); // f-2の呼び出し
    // @@@ sample end
    // clang-format on
}
}  // namespace ref_pattern2
