#include <compare>
#include <iostream>
#include <tuple>
#include <utility>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace auto_gen_member {
// @@@ sample begin 0:0

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

    ASSERT_TRUE(a == c);  // a == c
    ASSERT_TRUE(a != b);  // a != c
}
}  // namespace auto_gen_member

namespace auto_gen_non_member {
// @@@ sample begin 1:0

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
namespace use_spaceship {

// @@@ sample begin 2:0

struct Point {
    int x;
    int y;

    auto operator<=>(const Point& other) const noexcept = default;  // 三方比較演算子 (C++20)
    // 通常autoとするが、実際の戻り型はstd::strong_ordering
};
// @@@ sample end

TEST(ComparisonOp, spaceship)
{
    // @@@ sample begin 2:1

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

// @@@ sample begin 3:0

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
