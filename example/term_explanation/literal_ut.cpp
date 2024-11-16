#include <chrono>
#include <cmath>
#include <complex>
#include <regex>
#include <string>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

TEST(ExpTerm, raw_literal)
{
    // @@@ sample begin 0:0

    std::regex raw_re{R"(\d+)"};  // 生文字リテラルで正規表現パターン。\のエスケープが不要
    std::regex normal_re{"(\\d+)"};  // 生文字リテラルで正規表現パターン。\のエスケープが必要

    std::string test_str = "The year is 2024";  // テスト対象の文字列

    {
        std::smatch match;
        ASSERT_TRUE(std::regex_search(test_str, match, raw_re));  // 正規表現で数字部分を検索
        ASSERT_EQ(match.str(), "2024");  // マッチした部分が "2024" であることをチェック
    }
    {
        std::smatch match;
        ASSERT_TRUE(std::regex_search(test_str, match, normal_re));  // 正規表現で数字部分を検索
        ASSERT_EQ(match.str(), "2024");  // マッチした部分が "2024" であることをチェック
    }
    // @@@ sample end
}
TEST(ExpTerm, binary_literal)
{
    // @@@ sample begin 1:0

    int bin_value = 0b1101;  // 2進数リテラル  2進数1101 は10進数で 13
    ASSERT_EQ(bin_value, 13);
    // @@@ sample end

    // @@@ sample begin 2:0

    // 区切り文字を使った数値リテラル
    int large_number = 1'000'000;  // 10進数は3桁で区切るとわかりやすい
    ASSERT_EQ(large_number, 1000000);

    int bin_with_separator = 0b1011'0010;  // 10進数は4桁で区切るとわかりやすい
    ASSERT_EQ(bin_with_separator, 178);    // 2進数 1011 0010 は 10進数で 178

    int hex_with_separator = 0x00'00'ff'ff;  // 16進数は2桁で区切るとわかりやすい
    ASSERT_EQ(hex_with_separator, 65535);    // 16進数 0x00010011 == 65535
    // @@@ sample end
}

TEST(ExpTerm, wide_string)
{
    // clang-format off
    // @@@ sample begin 3:0

    // UTF-16 文字列リテラル（uプレフィックスを使用）
    char16_t       utf16_str[]  = u"こんにちは";
    std::u16string utf16_string = u"こんにちは";  // UTF-16 std::u16string 型

    // UTF-32 文字列リテラル（Uプレフィックスを使用）
    char32_t       utf32_str[]  = U"こんにちは";
    std::u32string utf32_string = U"こんにちは";  // UTF-32 std::u32string 型

#if __cplusplus >= 202002L  // c++20
    // UTF-8 文字列リテラル（u8プレフィックスを使用）
    const char8_t* utf8_str    = u8"こんにちは";
    std::u8string  utf8_string = u8"こんにちは";  // UTF-8 std::string 型

#else // c++17
    // UTF-8 文字列リテラル（u8プレフィックスを使用）
    const char* utf8_str    = "こんにちは";
    std::string utf8_string = "こんにちは";  // UTF-8 std::string 型
#endif
    // @@@ sample end
    // clang-format on

    IGNORE_UNUSED_VAR(utf16_str, utf16_string, utf32_str, utf32_str, utf8_string, utf8_str);
}

TEST(ExpTerm, hex_float)
{
    // @@@ sample begin 4:0

    // float型
    float hex_float = 0x1.2p3;
    EXPECT_FLOAT_EQ(hex_float, 9.0f);  // 正しい期待値は9.0f

    // double型
    double hex_double = 0x1.2p3;
    EXPECT_DOUBLE_EQ(hex_double, 9.0);  // 正しい期待値は9.0

    // 指数部が負の場合 (double型)
    double negative_exp = 0x1.2p-2;
    EXPECT_DOUBLE_EQ(negative_exp, 0.28125);  // 期待値は正しい

    // 小数点以下の部分がない場合 (double型)
    double integer_part = 0x1p3;
    EXPECT_DOUBLE_EQ(integer_part, 8.0);  // 期待値は正しい

    EXPECT_FLOAT_EQ(static_cast<float>(hex_double), hex_float);  // double型をfloatにキャスト
    // @@@ sample end
}

// @@@ sample begin 5:0

using namespace std::chrono_literals;

static_assert(1s == 1000ms);  // 1秒 (1s) は 1000 ミリ秒 (1000ms) と等しい

static_assert(1min == 60s);  // 1分 (1min) は 60秒 (60s) と等しい

static_assert(1h == 3600s);  // 1時間 (1h) は 3600秒 (3600s) と等しい

static_assert(1.5s == 1500ms);  // 小数点を使った時間リテラル
// @@@ sample end

TEST(ExpTerm, complex_literal)
{
    // @@@ sample begin 6:0

    using namespace std::complex_literals;  // 複素数リテラルを使うための名前空間

    auto a = 1.0 + 2.0i;  // std::complex<double>
    auto b = 3.0 + 4.0i;  // std::complex<double>

    auto result = a + b;
    EXPECT_EQ(result.real(), 4.0);
    EXPECT_EQ(result.imag(), 6.0);
    EXPECT_EQ(result, 4.0 + 6.0i);
    // @@@ sample end
}
}  // namespace
