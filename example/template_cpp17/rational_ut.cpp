#include <sstream>

#include "gtest_wrapper.h"

#include "rational.h"
#include "suppress_warning.h"

namespace Nstd {

TEST(RationalTest, int_op)
{
    // @@@ sample begin 0:0

    auto       r1    = Rational{1, 2};
    auto const r1_sv = r1;
    auto       r2    = Rational{1, 3};

    ASSERT_GE(r1, r2);

    r1 += r2;  // operator+
    ASSERT_EQ((Rational{5, 6}), r1);
    r1 = r1_sv;

    r1 -= r2;
    ASSERT_EQ((Rational{1, 6}), r1);
    r1 = r1_sv;

    auto oss = std::ostringstream{};
    oss << r1;
    ASSERT_EQ("1/2", oss.str());  // operator<<
    // @@@ sample end

    r1 *= r2;
    ASSERT_EQ((Rational{1, 6}), r1);
    r1 = r1_sv;

    r1 /= r2;
    ASSERT_EQ((Rational{3, 2}), r1);

    auto r3          = Rational{12, 2};
    auto compare_int = r3 == 6;

    ASSERT_TRUE(compare_int);

    r1 = -(r2 + r2 + r2);  // - (1/3 + 1/3 + 3/3)
    ASSERT_EQ(r1, -1);

    oss = std::ostringstream{};
    oss << r1;
    ASSERT_EQ("-1", oss.str());
}

TEST(RationalTest, literal)
{
    // @@@ sample begin 0:1

    auto r1 = "1/2"_rn;
    auto r2 = "1/3"_rn;

    ASSERT_GE(r1, r2);
    ASSERT_DOUBLE_EQ(static_cast<double>(r1), 0.5);
    // @@@ sample end
}

TEST(RationalTest, long_op)
{
    auto       r1    = Rational{1L, 2L};
    auto const r1_sv = r1;
    auto       r2    = Rational{1L, 3L};

    ASSERT_GE(r1, r2);

    r1 += r2;  // operator+
    ASSERT_EQ((Rational{5L, 6L}), r1);
    r1 = r1_sv;

    r1 -= r2;
    ASSERT_EQ((Rational{1L, 6L}), r1);
    r1 = r1_sv;

    auto oss = std::ostringstream{};
    oss << r1;
    ASSERT_EQ("1/2", oss.str());  // operator<<

    r1 *= r2;
    ASSERT_EQ((Rational{1L, 6L}), r1);
    r1 = r1_sv;

    r1 /= r2;
    ASSERT_EQ((Rational{3L, 2L}), r1);

    auto r3          = Rational{12L, 2L};
    auto compare_int = r3 == 6;

    ASSERT_TRUE(compare_int);

    r1 = -(r2 + r2 + r2);  // - (1/3 + 1/3 + 3/3)
    ASSERT_EQ(r1, -1L);

    oss = std::ostringstream{};
    oss << r1;
    ASSERT_EQ("-1", oss.str());
}
}  // namespace Nstd
