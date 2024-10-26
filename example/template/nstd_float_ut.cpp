#include <limits>

#include "gtest_wrapper.h"

namespace {

TEST(ProgrammingConvention, float_comp_0)
{
    // @@@ sample begin 0:0

    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    ASSERT_FALSE(0.05F == a + b);  // a + b == 0.05Fは一般には成立しない。
    // @@@ sample end
    // @@@ sample begin 0:1

    float epsilon  = std::abs(0.05F - (a + b));
    bool  is_equal = epsilon <= std::numeric_limits<float>::epsilon();
    ASSERT_TRUE(is_equal);
    // @@@ sample end
}
}  // namespace
// @@@ sample begin 1:0

namespace Nstd {

/// @brief float比較用関数
bool is_equal_f(float lhs, float rhs) noexcept
{
    return std::abs(lhs - rhs) <= std::numeric_limits<float>::epsilon();
}

/// @brief double比較用関数
bool is_equal_d(double lhs, double rhs) noexcept
{
    return std::abs(lhs - rhs) <= std::numeric_limits<double>::epsilon();
}
// @@@ sample end
}  // namespace Nstd
TEST(ProgrammingConvention, float_comp_1)
{
    // @@@ sample begin 1:1

    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    // floatの比較はis_equal_fのような関数を使う。
    bool is_equal = Nstd::is_equal_f(0.05F, a + b);
    ASSERT_TRUE(is_equal);
    // @@@ sample end
}
// @@@ sample end

TEST(ProgrammingConvention, float_comp_2)
{
    // @@@ sample begin 1:2

    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    // a + bはfloatの精度のまま、is_equal_dの引数の型であるdoubleに昇格される。
    // 一方、0.05はdoubleであるため(循環小数をdoubleの精度で切り捨てた値であるため)、
    // a + b(floatの精度の値)と0.05の差はdoubleのepsilonを超える。
    //  ASSERT_TRUE(is_equal_d(0.05, a + b));  // NG
    ASSERT_FALSE(Nstd::is_equal_d(0.05, a + b));
    // @@@ sample end
}

namespace fake {
// @@@ sample begin 2:0

namespace Nstd {
// is_equal_dを改良して、引数の型が統一されていない呼び出しをコンパイルエラーにできるようにした。
/// @brief 浮動小数点比較用関数
template <typename FLOAT_0, typename FLOAT_1>
bool is_equal(FLOAT_0 lhs, FLOAT_1 rhs) noexcept
{
    static_assert(std::is_floating_point_v<FLOAT_0>);
    static_assert(std::is_same_v<FLOAT_0, FLOAT_1>);

    return std::abs(lhs - rhs) <= std::numeric_limits<FLOAT_0>::epsilon();
}
}  // namespace Nstd
// @@@ sample end
TEST(ProgrammingConvention, float_comp_3)
{
    // @@@ sample begin 2:1

    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    // a + bはfloatであり、0.05はdoubleであるため、下記コードはコンパイルできない。
    // ASSERT_TRUE(Nstd::is_equal(0.05, a + b));
    ASSERT_TRUE(Nstd::is_equal(0.05F, a + b));  // OK リテラルに型を指定して、引数の型を統一
    // @@@ sample end
}
}  // namespace fake

namespace Nstd {
//
// @@@ sample begin 3:0

/// @brief 浮動小数点比較用関数
/// 相対誤差を考慮して、lhs と rhs の絶対値に基づくトレランスの範囲内での比較を行う。
/// これにより、数値の大小に関わらず、適切な誤差で比較できる。
template <typename FLOAT_0, typename FLOAT_1>
bool is_equal(FLOAT_0 lhs, FLOAT_1 rhs,
              FLOAT_0 tolerance = std::numeric_limits<FLOAT_0>::epsilon()) noexcept
{
    static_assert(std::is_floating_point_v<FLOAT_0>);
    static_assert(std::is_same_v<FLOAT_0, FLOAT_1>);

    return std::abs(lhs - rhs) <= tolerance * std::max(std::abs(lhs), std::abs(rhs));
}
// @@@ sample end
}  // namespace Nstd
namespace {
TEST(ProgrammingConvention, float_comp_4)
{
    // @@@ sample begin 3:1

    float a = 1000000.0F;
    float b = 1000000.1F;

    ASSERT_FALSE(Nstd::is_equal(a, b));       // a, bはほぼ同じだが。
    ASSERT_TRUE(Nstd::is_equal(a, b, 0.1F));  // 相対誤差を指定できる。
    // @@@ sample end
}
}  // namespace
