#include "gtest_wrapper.h"

#include "nstd_static_string.h"
#include "nstd_static_string_path.h"
#include "nstd_type2str.h"

namespace Nstd {
namespace {
TEST(Template, static_string_construct)
{
    {
        // @@@ sample begin 0:0

        const auto fs = StaticString{"abc"};  // C++17からのNの指定は不要

        static_assert(sizeof(4) == fs.Size());
        ASSERT_STREQ("abc", fs.String());

        // 文字列不足であるため、下記はコンパイルさせない
        // constexpr StaticString<4> fs2{"ab"};
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:1

        const auto fs = StaticString<4>{'a', 'b', 'c'};  // C++17でもNの指定は必要

        static_assert(sizeof(4) == fs.Size());
        ASSERT_STREQ("abc", fs.String());

        // 文字列不足であるため、下記はコンパイルさせない
        // constexpr StaticString<4> fs2{'a', 'b'};
        // @@@ sample end
    }
}

TEST(Template, TopStr_BottomStr)
{
    constexpr const char path[]{"0123456789"};
    {
        constexpr auto ss0 = StaticString{path};
        auto           ss3 = BottomStr<3>(ss0);

        static_assert(8 == ss3.Size());
        ASSERT_STREQ("3456789", ss3.String());

        auto ss8 = BottomStr<8>(ss0);

        static_assert(3 == ss8.Size());
        ASSERT_STREQ("89", ss8.String());
    }
    {
        constexpr auto ss0 = StaticString{path};
        auto           ss3 = TopStr<3>(ss0);

        static_assert(3 == ss3.Size());
        ASSERT_STREQ("01", ss3.String());

        auto ss8 = TopStr<8>(ss0);

        static_assert(8 == ss8.Size());
        ASSERT_STREQ("0123456", ss8.String());
    }
}

TEST(Template, static_string_eq)
{
    // @@@ sample begin 1:0

    static_assert(StaticString{"123"} == StaticString{"123"});
    static_assert(StaticString{"123"} != StaticString{"1234"});
    static_assert(StaticString{"123"} == "123");
    static_assert("123" == StaticString{"123"});
    static_assert(StaticString{"123"} != "1234");
    static_assert("1234" != StaticString{"123"});
    // @@@ sample end
}

TEST(Template, static_string_plus)
{
    // @@@ sample begin 2:0

    constexpr auto fs0 = StaticString{"1234"} + StaticString{"567"};
    static_assert(std::is_same_v<StaticString<fs0.Size()> const, decltype(fs0)>);
    static_assert("1234567" == fs0);

    constexpr auto fs1 = StaticString{"1234"} + ":";
    static_assert(std::is_same_v<StaticString<fs1.Size()> const, decltype(fs1)>);
    static_assert("1234:" == fs1);

    constexpr auto fs2 = ":" + StaticString{"567"};
    static_assert(std::is_same_v<StaticString<fs2.Size()> const, decltype(fs2)>);
    static_assert(":567" == fs2);

    constexpr auto fs3 = StaticString{"1234"} + ":" + StaticString{"567"};
    static_assert(std::is_same_v<StaticString<fs3.Size()> const, decltype(fs3)>);
    static_assert("1234:567" == fs3);
    // @@@ sample end
}
}  // namespace
}  // namespace Nstd
