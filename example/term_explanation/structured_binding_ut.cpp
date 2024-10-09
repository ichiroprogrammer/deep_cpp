#include <tuple>
#include <type_traits>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {

TEST(TermExp, structed_binding1)
{
    // @@@ sample begin 0:0

    // tupleでの構造化束縛の例
    std::tuple<int, double, std::string> tobj(1, 2.5, "Hello");

    auto [i, d, s] = tobj;  // 構造化束縛を使用してタプルを分解

    ASSERT_EQ(i, 1);
    ASSERT_DOUBLE_EQ(d, 2.5);
    ASSERT_EQ("Hello", s);
    // @@@ sample end
}

TEST(TermExp, structed_binding2)
{
    // @@@ sample begin 0:1

    // pairでの構造化束縛の例
    std::pair<int, std::string> pobj(42, "example");

    auto [i, s] = pobj;  // 構造化束縛を使用してペアを分解

    ASSERT_EQ(i, 42);
    ASSERT_EQ("example", s);
    // @@@ sample end
}

TEST(TermExp, structed_binding3)
{
    // @@@ sample begin 0:2

    struct Person {
        std::string name;
        int         age;
    };

    Person person{"Ichiro", 30};  // 構造体のインスタンス

    auto& [name, age] = person;  // 構造化束縛を使用して構造体のメンバーを分解

    static_assert(std::is_same_v<decltype(name), std::string>);  // これは正しい
    // static_assert(std::is_same_v<decltype(name), std::string&>); これはコンパイルエラー
    // 上記がコンパイルエラーになる理由は以下の通り。
    //
    // 変数宣言は、 autoを記述したあとに角カッコ内に変数名を列挙する。
    // それぞれの変数に対する型や修飾子の指定はできない。
    // autoの部分を const auto&のように、全体に対してCV修飾や参照を付加することはできる。
    // それぞれの変数の型は、各要素をdecltypeしたものとなる。
    //                        ^^^^^^^^^^^^^^ nameの型がstd::stringと評価された理由

    name = "Taro";  // nameはリファレンス
    age  = 56;      // ageはリファレンス
    ASSERT_EQ(person.name, "Taro");
    ASSERT_EQ(person.age, 56);
    // @@@ sample end
}

TEST(TermExp, structed_binding4)
{
    // @@@ sample begin 0:3

    auto array = std::array<int, 3>{1, 2, 3};

    auto [x, y, z] = array;  // 構造化束縛を使って std::array の要素を分解

    ASSERT_EQ(x, 1);
    ASSERT_EQ(y, 2);
    ASSERT_EQ(z, 3);
    // @@@ sample end
}
}  // namespace
