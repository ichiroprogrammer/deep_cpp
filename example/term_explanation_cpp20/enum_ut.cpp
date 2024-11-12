#include <cassert>
#include <cstddef>
#include <string_view>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
TEST(TermExp, enum_normal)
{
    SUPPRESS_WARN_BEGIN;
    SUPPRESS_WARN_ENUM_COMP;
    // @@@ sample begin 0:0

    enum DayOfWeek { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

    ASSERT_TRUE(1 == Monday);  // intへの暗黙の変換

    enum Color { Red, Green, Blue };

    ASSERT_TRUE(Green == Monday);  // 別のenumが比較できてしまう
    // @@@ sample end
    SUPPRESS_WARN_END;
}

TEST(TermExp, enum_class)
{
    // @@@ sample begin 1:0

    enum class DayOfWeek { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

    // ASSERT_TRUE(1 == Monday);  // intへの暗黙の変換できないため、コンパイルエラー
    ASSERT_TRUE(1 == static_cast<int>(DayOfWeek::Monday));

    enum class Color { Red, Green, Blue };

    // ASSERT_TRUE(Green == Monday);  // 別のenumが比較できないため、コンパイルエラー
    ASSERT_TRUE(static_cast<DayOfWeek>(Color::Green) == DayOfWeek::Monday);
    // @@@ sample end
    // @@@ sample begin 1:1

    // DayOfWeek d0 {0}; intからの暗黙の型変換は許可されないため、コンパイルエラー
    DayOfWeek d0{static_cast<DayOfWeek>(0)};
    DayOfWeek d1{};  // デフォルト初期化
    ASSERT_EQ(d1, DayOfWeek::Sunday);

    DayOfWeek d2{DayOfWeek::Tuesday};  // 値あり初期化
    // @@@ sample end

    IGNORE_UNUSED_VAR(d0, d1, d2);
}

// @@@ sample begin 2:0

enum NormalEnum {  // underlying typeの指定しない従来のenum
};

enum NormalEnumUnderlyingType : int8_t {  // enum underlying typeがint8_tに指定された従来のenum
};

// enum class
enum class EnumClass {  // underlying typeの指定しないenum class
};

enum class EnumClassUnderlyingType : int64_t {  // enum underlying typeがint64_tに指定されたenum
                                                // class
};

// clang-format off
static_assert(4 ==  sizeof(NormalEnum));  // 列挙子の値を表現するのに十分なサイズの整数型で処理系依存
static_assert(4 ==  sizeof(EnumClass));   // 列挙子の値を表現するのに十分なサイズの整数型で処理系依存
// clang-format on
static_assert(sizeof(int8_t) == sizeof(NormalEnumUnderlyingType));
static_assert(sizeof(int64_t) == sizeof(EnumClassUnderlyingType));
// @@@ sample end

TEST(TermExp, enum_class2)
{
    // @@@ sample begin 2:1

    enum class Color : int { Red, Green, Blue };

    // Color red{0}; C++14まではコンパイルエラー

    Color red{0};  // underlying typeの効果でC++17からコンパイルできる。

    long a{1};
    // Color green{a};  // 縮小型変換が発生するため、コンパイルエラー

    // @@@ sample end
    IGNORE_UNUSED_VAR(red, a);
}

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_UNUSED_FUNC;
// @@@ sample begin 3:0

// in calender.h
enum class DayOfWeek : int8_t;  // DayOfWeekの前方宣言

bool calender(DayOfWeek);  // 前方宣言の効果でこのヘッダでの#include "day_of_week.h"が不要

// in day_of_week.h
enum class DayOfWeek : int8_t { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
// @@@ sample end

SUPPRESS_WARN_END;
static_assert(sizeof(calender(DayOfWeek::Sunday)) == 1);

TEST(TermExp, std_byte)
{
    // @@@ sample begin 4:0

    uint8_t u8_0     = 0x80;
    auto    result_0 = u8_0 << 1;  // 汎整数拡張のためresult_0の型はintになる

    static_assert(std::is_same_v<decltype(result_0), int>);
    ASSERT_EQ(0x100, result_0);  // これがわかりずらいバグにつながることがある

    auto u8_1     = std::byte{0x80};
    auto result_1 = u8_1 << 1;  // 汎整数拡張は発生せず、result_1の型はstd::byteになる

    static_assert(std::is_same_v<decltype(result_1), std::byte>);

    // 整数型を取り出すためには、暗黙の型変換ではなく、
    // 明示的なto_integerの呼び出しが必要になることもコードの安全性につながる
    ASSERT_EQ(0x00, to_integer<int>(result_1));  // 0x100はstd::byteでは0
    // @@@ sample end
}

namespace no_using {
// @@@ sample begin 5:0

enum class Color { Red, Green, Yellow };

constexpr std::string_view to_str(Color color)
{
    switch (color) {
    case Color::Red:
        return "Red";
    case Color::Green:
        return "Green";
    case Color::Yellow:
        return "Yellow";
    default:
        assert(false);
    }

    return "";
}
// @@@ sample end

TEST(TermExp, to_str) { ASSERT_EQ("Red", to_str(Color::Red)); }
}  // namespace no_using

namespace enum_using {
// @@@ sample begin 5:0

enum class Color { Red, Green, Yellow };

constexpr std::string_view to_str(Color color)
{
    using enum Color;  // 名前修飾の省略可能にする

    switch (color) {
    case Red:
        return "Red";
    case Green:
        return "Green";
    case Yellow:
        return "Yellow";
    }

    assert(false);
    return "";
}
// @@@ sample end

TEST(TermExp, to_str)
{
    // @@@ sample begin 5:1

    using Color::Red;  // Redに関しては名前修飾なしで使用する

    ASSERT_EQ("Red", to_str(Red));
    ASSERT_EQ("Yellow", to_str(Color::Yellow));
    // @@@ sample end
}
}  // namespace enum_using

namespace enum_using2 {
// @@@ sample begin 6:0

class Signal {
public:
    enum class Color { Red, Green, Yellow };
    using enum Color;  // 列挙子を簡潔に書くためのusing

    void Set(Color);

private:
    // ...
};
// @@@ sample end

TEST(TermExp, to_str)
{
    // @@@ sample begin 6:1

    Signal s{};

    s.Set(Signal::Color::Red);  // 名前修飾が長すぎる感じがする
    s.Set(Signal::Red);         // using enum colorがあるために、簡潔に書ける

    using Signal::Red;  // Redに関しては名前修飾なしで使用する
                        // この記述によりこの名前空間でのRed識別子が使えなくなる
    s.Set(Red);
    // @@@ sample end

    IGNORE_UNUSED_VAR(s);
}
void Signal::Set(Signal::Color) {}
}  // namespace enum_using2
}  // namespace
