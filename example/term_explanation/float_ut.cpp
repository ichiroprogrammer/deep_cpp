#include <cfenv>
#include <cmath>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace float_term_exp {

TEST(ExpTerm, rounding_error)
{
    // @@@ sample begin 0:0

    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    //  ASSERT_EQ(0.05F, a + b);  // NG  a + b == 0.05Fは一般には成立しない。
    ASSERT_NE(0.05F, a + b);
    // @@@ sample end
}
TEST(ExpTerm, epsilon)
{
    // @@@ sample begin 1:0

    // 下記の0.01は2進数では循環小数となるため、実数の0.01とは異なる。
    constexpr auto a = 0.01F;  // 0.0000001010001111...
    constexpr auto b = 0.04F;  // 0.0000101000111101...

    bool is_equal = 0.05F == (a + b);
    ASSERT_FALSE(is_equal);  // is_equalはtrueにはならない

    bool is_nearly_equal = std::abs(0.05F - (a + b)) <= std::numeric_limits<float>::epsilon();
    ASSERT_TRUE(is_nearly_equal);  // 浮動小数点の同値はこのように判定する
    // @@@ sample end
}

TEST(ExpTerm, float_error)
{
    // @@@ sample begin 2:0

    std::feclearexcept(FE_ALL_EXCEPT);  // 全ての例外フラグをクリア

    double result0 = 1.0 / 0.0;                    // ゼロ除算を引き起こす
    EXPECT_TRUE(std::fetestexcept(FE_DIVBYZERO));  // ゼロ除算例外が発生したか確認

    std::feclearexcept(FE_ALL_EXCEPT);  // 全ての例外フラグをクリア

    double result1 = std::numeric_limits<double>::max() * 10.0;  // 最大値を超えてオーバーフロー
    EXPECT_TRUE(std::fetestexcept(FE_OVERFLOW));  // オーバーフロー例外が発生したか確認

    std::feclearexcept(FE_ALL_EXCEPT);  // 全ての例外フラグをクリア

    double result2 = std::pow(10.0, -308);         // アンダーフローを引き起こす
    EXPECT_TRUE(std::fetestexcept(FE_UNDERFLOW));  // アンダーフロー例外が発生したか確認
    // @@@ sample end
    IGNORE_UNUSED_VAR(result0, result1, result2);
}

}  // namespace float_term_exp
