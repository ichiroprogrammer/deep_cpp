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
    // @@@ sample begin 0:1

    using namespace std::rel_ops;  // std::rel_opsを使うために名前空間を追加

    auto a = Integer{5};
    auto b = Integer{10};
    auto c = Integer{5};

    // std::rel_opsとは無関係に直接定義
    ASSERT_EQ(a, c);      // a == c
    ASSERT_NE(a, b);      // a != c
    ASSERT_TRUE(a < b);   // aはbより小さい
    ASSERT_FALSE(b < a);  // bはaより小さくない

    // std::rel_ops による!=, <=, >, >=の定義
    ASSERT_TRUE(a != b);   // aとbは異なる
    ASSERT_TRUE(a <= b);   // aはb以下
    ASSERT_TRUE(b > a);    // bはaより大きい
    ASSERT_FALSE(a >= b);  // aはb以上ではない
    // @@@ sample end
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

    // @@@ sample begin 1:2

    using namespace std::rel_ops;  // std::rel_opsを使うために名前空間を追加

    auto a = Integer{5};
    auto b = Integer{10};
    auto c = Integer{5};

    // std::rel_opsとは無関係に直接定義
    ASSERT_EQ(a, c);      // a == c
    ASSERT_NE(a, b);      // a != c
    ASSERT_TRUE(a < b);   // aはbより小さい
    ASSERT_FALSE(b < a);  // bはaより小さくない

    // std::rel_ops による!=, <=, >, >=の定義
    ASSERT_TRUE(a != b);   // aとbは異なる
    ASSERT_TRUE(a <= b);   // aはb以下
    ASSERT_TRUE(b > a);    // bはaより大きい
    ASSERT_FALSE(a >= b);  // aはb以上ではない
    // @@@ sample end
}
}  // namespace Use_rel_ops2
namespace use_tupple {

// @@@ sample begin 2:0

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const noexcept { return std::tie(x, y) == std::tie(other.x, other.y); }

    bool operator<(const Point& other) const noexcept { return std::tie(x, y) < std::tie(other.x, other.y); }
};
// @@@ sample end

TEST(ComparisonOp, tupple)
{
    // @@@ sample begin 2:1

    auto a = Point{1, 2};
    auto b = Point{1, 3};
    auto c = Point{1, 2};

    using namespace std::rel_ops;  // std::rel_opsを使うために名前空間を追加

    ASSERT_TRUE(a == c);
    ASSERT_TRUE(a != b);
    ASSERT_TRUE(a < b);
    ASSERT_FALSE(a > b);
    // @@@ sample end
}
}  // namespace use_tupple
namespace use_spaceship {

// @@@ sample begin 3:0

struct Point {
    int x;
    int y;

    auto operator<=>(const Point& other) const noexcept = default;  // 三方比較演算子 (C++20)
    // 通常autoとするが、実際の戻り型はstd::strong_ordering
};
// @@@ sample end

TEST(ComparisonOp, spaceship)
{
    // @@@ sample begin 3:1

    auto p1 = Point{1, 2};
    auto p2 = Point{1, 2};
    auto p3 = Point{2, 3};

    ASSERT_EQ(p1, p2);  // p1 == p2
    ASSERT_NE(p1, p3);  // p1 != p3
    ASSERT_TRUE(p1 < p3);
    ASSERT_FALSE(p1 > p3);

    auto cmp_1_2 = p1 <=> p2;
    auto cmp_1_3 = p1 <=> p3;
    auto cmp_3_1 = p3 <=> p1;
    static_assert(std::is_same_v<std::strong_ordering, decltype(cmp_1_2)>);

    ASSERT_EQ(std::strong_ordering::equal, cmp_1_2);    // 等しい
    ASSERT_EQ(std::strong_ordering::less, cmp_1_3);     // <=>の左オペランドが小さい
    ASSERT_EQ(std::strong_ordering::greater, cmp_3_1);  // <=>の左オペランドが大きい

    // std::strong_orderingの値
    // ASSERT_EQ(static_cast<int32_t>(cmp_1_2), 0); キャストできないのでコンパイルエラー
    ASSERT_TRUE(cmp_1_2 == 0);
    ASSERT_TRUE(cmp_1_3 < 0);  // cmp_1_3は実質的には-1
    ASSERT_TRUE(cmp_3_1 > 0);  // cmp_3_1は実質的には1

    // @@@ sample end
}
}  // namespace use_spaceship
namespace use_spaceship2 {

// @@@ sample begin 4:0

struct Point {
    int x;
    int y;

    std::strong_ordering operator<=>(const Point& other) const noexcept
    {
        return std::tie(x, y) <=> std::tie(other.x, other.y);
    }

    bool operator==(const Point& other) const noexcept { return std::tie(x, y) == std::tie(other.x, other.y); }
};
// @@@ sample end

TEST(ComparisonOp, spaceship)
{
    // @@@ sample begin 4:1

    auto p1 = Point{1, 2};
    auto p2 = Point{1, 2};
    auto p3 = Point{2, 3};

    ASSERT_EQ(p1, p2);  // p1 == p2
    ASSERT_NE(p1, p3);  // p1 != p3
    ASSERT_TRUE(p1 < p3);
    ASSERT_FALSE(p1 > p3);
    // @@@ sample end
}
}  // namespace use_spaceship2

namespace auto_gen_member {
// @@@ sample begin 5:0

class Integer {
public:
    Integer(int x) noexcept : x_{x} {}

    bool operator==(const Integer& other) const noexcept = default;  // 自動生成

private:
    int x_;
};
// @@@ sample end
TEST(ComparisonOp, auto_gen_member)
{
    auto a = Integer{5};
    auto b = Integer{10};
    auto c = Integer{5};

    ASSERT_EQ(a, c);  // a == c
    ASSERT_NE(a, b);  // a != c
}
}  // namespace auto_gen_member

namespace auto_gen_non_member {
// @@@ sample begin 6:0

class Integer {
public:
    Integer(int x) noexcept : x_{x} {}

    friend bool operator==(Integer const& lhs, Integer const& rhs) noexcept;

private:
    int x_;
};

bool operator==(Integer const& lhs, Integer const& rhs) noexcept = default;  // 自動生成
// @@@ sample end
TEST(ComparisonOp, auto_gen_non_member)
{
    auto a = Integer{5};
    auto b = Integer{10};
    auto c = Integer{5};

    ASSERT_TRUE(a == c);  // a == c
    ASSERT_NE(a, b);      // a != c
}
}  // namespace auto_gen_non_member
