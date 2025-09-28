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
    auto   sp = &rvalue_ref;  // spはrvalue_refのアドレスを指しているが、、、
    // @@@ sample end

    static_assert(std::is_same_v<std::string*, decltype(sp)>);
    IGNORE_UNUSED_VAR(s, sp);
}
}
namespace ref_pattern1 {
// clang-format off
char const* f(std::string&)       { return "lvalue ref"; }
char const* f(std::string const&) { return "lvalue const ref"; }
// clang-format on

TEST(Expression, rvalue_ref1)
{
    std::string       str;
    std::string const cstr;

    ASSERT_STREQ("lvalue ref", f(str));
    ASSERT_STREQ("lvalue const ref", f(cstr));
    ASSERT_STREQ("lvalue const ref",
                 f(std::string{}));  // T const&はrvalueリファレンスをバインドできる
}
}  // namespace ref_pattern1

namespace ref_pattern2 {
// clang-format off
char const* f(std::string&)       { return "lvalue ref"; }
char const* f(std::string const&) { return "lvalue const ref"; }
char const* f(std::string&&)      { return "rvalue ref"; }
// clang-format on

TEST(Expression, rvalue_ref2)
{
    std::string       str;
    std::string const cstr;

    ASSERT_STREQ("lvalue ref", f(str));
    ASSERT_STREQ("lvalue const ref", f(cstr));
    ASSERT_STREQ("rvalue ref", f(std::string{}));
    ASSERT_STREQ("rvalue ref", f(std::string{}));
    ASSERT_STREQ("rvalue ref", f(std::move(str)));
    ASSERT_STREQ("lvalue const ref", f(std::move(cstr)));
}
}  // namespace ref_pattern2
