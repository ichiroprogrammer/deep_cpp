#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Rvalue {

void f()

{
    // @@@ sample begin 0:0
    // str0を初期化するためにstd::string{}により生成されるオブジェクトはprvalue、 str0はlvalue
    //   ↓lvalue
    auto str0 = std::string{};  // この式の左辺はテンポラリオブジェクト(つまりprvalue)

    /*
    auto* str0_ptr = &std::string{};  // prvalueのアドレスの取得はできない
    ↑は、メッセージは error: taking address of rvalue でコンパイルエラー */

    /*
    std::string& str1_ref = std::string{};  // prvalueを非constなlvalueリファレンスではバインドできない
    ↑は、コンパイルエラーで、エラーメッセージは error: taking address of rvalue */

    std::string const& str2_ref = std::string{};  // prvalueはconstなlvalueリファレンスでバインドできる
    // ↓のようにすればアドレスを取得できるが、このようなことはすべきではない。
    std::string const* str2_ptr = &str2_ref;  // str_ptrはprvalueのアドレスを指しているが、、、

    auto&& str3_ref = std::string{};  // prvalueはprvalueリファレンスでバインドできる
    // ↓のようにすればアドレスを取得できるが、このようなことはすべきではない。
    std::string* str3_ptr = &str3_ref;  // str_ptrはprvalueのアドレスを指しているが、、、
    // @@@ sample end

    IGNORE_UNUSED_VAR(str0, str2_ptr, str3_ref, str3_ptr);
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
    int& t_ref = int{99};  非const lvalueリファレンスはrvalueをバインドできない */
    int const& t_ref = int{99};  // 上記とは異なり、const lvalueリファレンスはrvalueをバインドできる
    ASSERT_EQ(t_ref, 99);
    // @@@ sample end
}

// clang-format off
// @@@ sample begin 1:1

int f(int& )        { return 1; }   // f-1
int f(int const & ) { return 2; }   // f-2
// @@@ sample end
// clang-format on

TEST(Expression, lvalue_ref1)
{
    // @@@ sample begin 1:2

    int       a = 0;
    int const b = 0;

    ASSERT_EQ(1, f(a));  // f(a)は、f-2も呼び出せるが、デフォルトでは、f-1が呼ばれる
    ASSERT_EQ(2, f(static_cast<int const&>(a)));  // aをconstにキャストして、強制的にf-2の呼び出し
    ASSERT_EQ(2, f(b));                           // constオブジェクトのバインド
    ASSERT_EQ(2, f(int{}));                       // rvalueのバインド
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
}  // namespace ref_pattern2

namespace ref_pattern3 {
int f(int&)       { return 1; } // f-1
int f(int const&) { return 2; } // f-2
int f(int&&)      { return 3; } // f-3
// @@@ sample end
// clang-format on

TEST(Expression, rvalue_ref2)
{
    // clang-format off
    // @@@ sample begin 2:2

    int       a = 0;
    int const b = 0;

    ASSERT_EQ(1, f(a));      // f-1の呼び出し
    ASSERT_EQ(2, f(b));      // f-2の呼び出し、constなlvalueリファレンスのバインド
    ASSERT_EQ(3, f(int{}));  // f-3の呼び出し、f-3が無ければ、f-2を呼び出すが
    ASSERT_EQ(2, f(static_cast<int const&>(a)));  // strをconstリファレンスにキャストして、強制的にf-2の呼び出し
    ASSERT_EQ(3, f(static_cast<int&&>(a)));       // strをrvalueリファレンスにキャストして、 強制的にf-3の呼び出し
    ASSERT_EQ(3, f(std::move(a)));                // f-3の呼び出し
    // @@@ sample end
    // clang-format on
}
}  // namespace ref_pattern3
