#include <iterator>
#include <list>
#include <sstream>
#include <string_view>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"

namespace RangeFor {
TEST(RangeFor, range_for)
{
    {
        // @@@ sample begin 0:0

        auto list = std::list{1, 2, 3};
        auto oss  = std::stringstream{};

        for (auto a : list) {  // 範囲for文
            oss << a;
        }
        ASSERT_EQ(oss.str(), "123");
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:1

        auto list = std::list{1, 2, 3};
        auto oss  = std::stringstream{};

        // std::begin(list)、std::end(list)の戻り型が同一
        static_assert(std::is_same_v<decltype(std::begin(list)), decltype(std::end(list))>);

        // 上記の範囲for文は下記のように展開される
        for (auto it = std::begin(list); it != std::end(list); ++it) {
            oss << *it;
        }

        ASSERT_EQ(oss.str(), "123");
        // @@@ sample end
    }
}

// delimiter や終端に到達したかどうか判定する述語
template <char delimiter>
struct end_of_delimited_string {
    // イテレータが delimiter もしくは '\0' でないかを判定する
    bool operator()(std::string::iterator it) { return *it != delimiter && *it != '\0'; }
};

// 区切り文字を扱うクラス
template <char delimiter>
struct delimited_string {
    std::string str;

    // delimited_string::begin と delimited_string::end の型は異なる
    std::string::iterator begin() { return str.begin(); }

    // 終了条件としてend_of_delimited_string を返す
    end_of_delimited_string<delimiter> end() const { return end_of_delimited_string<delimiter>(); }
};

// イテレータと終了条件の比較を行う演算子オーバーロード
template <char delimiter>
bool operator!=(std::string::iterator it, end_of_delimited_string<delimiter> e)
{
    return e(it);
}

TEST(ProgrammingConvention, delimited_string)
{
    {
        // @@@ sample begin 2:0

        delimited_string<','> delimited_str{"Hello,World"};
        std::ostringstream    oss;

        // ',' を終了文字として "Hello" だけをループして出力
        for (auto c : delimited_str) {
            oss << c;
        }

        ASSERT_EQ("Hello", oss.str());  // 結果は "Hello" になるはず
        // @@@ sample end
    }
    {
        // @@@ sample begin 2:1

        delimited_string<','> delimited_str{"Hello,World"};
        std::ostringstream    oss;

        // ',' を終了文字として"Hello" だけをループして出力
        for (auto it = delimited_str.begin(); it != delimited_str.end(); ++it) {
            oss << *it;
        }

        ASSERT_EQ("Hello", oss.str());  // 結果は "Hello" になるはず
        // @@@ sample end
    }
}
// string_view
TEST(ProgrammingConvention, string_view)
{
    auto       oss = std::ostringstream{};
    char const abc[]{"abc"};

    ASSERT_EQ("char const [4]", Nstd::Type2Str<decltype(abc)>());

    for (char c : abc) {
        oss << c;
    }

    ASSERT_EQ((std::string{'a', 'b', 'c', '\0'}), oss.str());  // ヌル文字が入る

    oss         = std::ostringstream{};
    auto abc_sv = std::string_view{abc};

    for (char c : abc_sv) {
        oss << c;
    }

    ASSERT_EQ((std::string{'a', 'b', 'c'}), oss.str());  // ヌル文字は入らない
}
}  // namespace RangeFor
