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

        auto const fs = StaticString{"abc"};  // C++17からのNの指定は不要

        static_assert(sizeof(4) == fs.Size());
        ASSERT_STREQ("abc", fs.String());

        // 文字列不足であるため、下記はコンパイルさせない
        // constexpr StaticString<4> fs2{"ab"};
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:1

        auto const fs = StaticString<4>{'a', 'b', 'c'};  // C++17でもNの指定は必要

        static_assert(sizeof(4) == fs.Size());
        ASSERT_STREQ("abc", fs.String());

        // 文字列不足であるため、下記はコンパイルさせない
        // constexpr StaticString<4> fs2{'a', 'b'};
        // @@@ sample end
    }
}

TEST(Template, TopStr_BottomStr)
{
    {
        constexpr char const str[]{"0123456789"};
        constexpr auto       ss0 = StaticString{str};
        auto                 ss3 = BottomStr<3>(ss0);

        static_assert(8 == ss3.Size());
        ASSERT_STREQ("3456789", ss3.String());

        auto ss8 = BottomStr<8>(ss0);

        static_assert(3 == ss8.Size());
        ASSERT_STREQ("89", ss8.String());
    }

    // @@@ sample begin 0:2

    constexpr auto ss  = StaticString{"0123456789"};
    auto           ss2 = TopStr<2>(ss);  // 先頭2文字
    static_assert(3 == ss2.Size());      // 先頭2文字 + 終端文字
    ASSERT_STREQ("01", ss2.String());

    auto ss8 = BottomStr<2>(ss);     // 先頭からオフセット2文字～終端文字まで
    static_assert(9 == ss8.Size());  // 先頭からオフセット2文字～終端文字までは結果的に9文字
    ASSERT_STREQ("23456789", ss8.String());

    ASSERT_EQ(ss2 + ss8, ss);  // 元に戻す。+、= が使用される。
    // @@@ sample end
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
