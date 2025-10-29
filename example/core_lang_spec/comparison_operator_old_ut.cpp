#include <compare>
#include <iostream>
#include <tuple>
#include <utility>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Use_rel_ops {

// @@@ sample begin 0:0

class Integer {
public:
    Integer(int x) noexcept : x_{x} {}

    // operator==とoperator<だけを定義
    int get() const noexcept { return x_; }

    // メンバ関数の比較演算子
    bool operator==(const Integer& other) const noexcept { return x_ == other.x_; }
    bool operator<(const Integer& other) const noexcept { return x_ < other.x_; }

private:
    int x_;
};
// @@@ sample end

TEST(ComparisonOp, rel_ops)
{
    using namespace std::rel_ops;  // std::rel_opsを使うために名前空間を追加

    auto a = Integer{5};
    auto b = Integer{10};
    auto c = Integer{5};

    // std::rel_opsとは無関係に直接定義
    ASSERT_EQ(a, c);       // a == c
    ASSERT_FALSE(a == b);  // !(a == b)
    ASSERT_TRUE(a < b);    // aはbより小さい
    ASSERT_FALSE(b < a);   // bはaより小さくない

    // std::rel_ops による!=, <=, >, >=の定義
    ASSERT_TRUE(a != b);   // aとbは異なる
    ASSERT_TRUE(a <= b);   // aはb以下
    ASSERT_TRUE(b > a);    // bはaより大きい
    ASSERT_FALSE(a >= b);  // aはb以上ではない
}
}  // namespace Use_rel_ops
namespace Use_rel_ops2 {

// @@@ sample begin 1:0

class Integer {
public:
    Integer(int x) noexcept : x_{x} {}

    // operator==とoperator<だけを定義
    int get() const noexcept { return x_; }

    // メンバ関数の比較演算子に見えるが、非メンバ関数
    friend bool operator==(const Integer& lhs, const Integer& rhs) noexcept { return lhs.x_ == rhs.x_; }

    friend bool operator<(const Integer& lhs, const Integer& rhs) noexcept { return lhs.x_ < rhs.x_; }

private:
    int x_;
};
// @@@ sample end

TEST(ComparisonOp, rel_ops_non_member)
{
    {
        // @@@ sample begin 1:1

        auto a = Integer{5};

        ASSERT_TRUE(5 == a);  // 5がInteger{5}に型型変換される
        // @@@ sample end
    }

    using namespace std::rel_ops;  // std::rel_opsを使うために名前空間を追加

    auto a = Integer{5};
    auto b = Integer{10};
    auto c = Integer{5};

    // std::rel_opsとは無関係に直接定義
    ASSERT_EQ(a, c);       // a == c
    ASSERT_FALSE(a == b);  // a != c
    ASSERT_TRUE(a < b);    // aはbより小さい
    ASSERT_FALSE(b < a);   // bはaより小さくない

    // std::rel_ops による!=, <=, >, >=の定義
    ASSERT_TRUE(a != b);   // aとbは異なる
    ASSERT_TRUE(a <= b);   // aはb以下
    ASSERT_TRUE(b > a);    // bはaより大きい
    ASSERT_FALSE(a >= b);  // aはb以上ではない
}
}  // namespace Use_rel_ops2
