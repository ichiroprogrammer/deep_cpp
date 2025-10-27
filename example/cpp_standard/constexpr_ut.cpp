#include <string>
#include <type_traits>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {

// @@@ sample begin 1:0

template <int N>
struct Templ {
    static constexpr auto value = N;  // valueは定数
};
// @@@ sample end
TEST(TermExp, constexpr_constant)
{
    // @@@ sample begin 1:1

    constexpr int a = 5;  // aは定数であるためかきのような使い方ができる
    static_assert(a == 5);

    constexpr int b = 5;  // bは定数でないため、下記のような使い方ができない
    // static_assert(b == 5);  // コンパイルエラー

    constexpr double PI{3.14159265358979323846};  // PIはconstexpr

    auto templ = Templ<a>{};  // aはconstexprなのでaの初期化が可能

    static_assert(templ.value == 5);
    // @@@ sample end

    IGNORE_UNUSED_VAR(a, b, PI);
}

// clang-format off
// @@@ sample begin 2:0

constexpr int f(int a) noexcept { return a * 3; }  // aがconstexprならばf(a)もconstexpr
int g(int a) noexcept { return a * 3; }            // aがconstexprであってもg(a)は非constexpr
// @@@ sample end
// clang-format on

TEST(TermExp, constexpr_func)
{
    // clang-format off
    // @@@ sample begin 2:1

    auto x = int{0};

    constexpr auto a = f(3);     // f(3)はconstexprなのでaの初期化が可能
    // constexpr auto b = f(x);  // xは非constexprなのでbの初期化はコンパイルエラー
    auto const c = f(3);         // cはconstexpr定数と定義とすべき
    // constexpr auto d = g(3);  // g(3)は非constexprなのでdの初期化はコンパイルエラー
    auto const e = g(x);         // eはここで初期化して、この後不変
    // @@@ sample end
    // clang-format on

    IGNORE_UNUSED_VAR(x, a, c, e);
}

// clang-format off
// @@@ sample begin 3:0

class Integer {
public:
    constexpr Integer(int32_t integer) noexcept : integer_{integer} {}
    constexpr operator int() const noexcept { return integer_; }  // constexprメンバ関数はconst
    constexpr int32_t Allways2() const noexcept { return 2; }     // constexprメンバ関数はconst
    static constexpr int32_t Allways3() noexcept { return 3; }    // static関数のconstexpr化

private:
    int32_t integer_;
};
// @@@ sample end
// clang-format on

TEST(TermExp, constexpr_instance)
{
    // clang-format off
    // @@@ sample begin 3:1

    constexpr auto i5 = 5;                // i5はconstexprインスタンス
    constexpr auto int_5 = Integer{i5};   // int_5はconstexprインスタンス
    static_assert(int_5 == 5);            // intへの暗黙の型変換

    auto i3  = 3;                         // i3はconstexpr定数ではない
    auto int_3 = Integer{i3};             // int_3はconstexprインスタンスではない
    // static_assert(int_3 == 5);         // int_3がconstexprではないため、コンパイルエラー
    static_assert(int_3.Allways2() == 2); // int_3はconstexprインスタンスではないが、
                                          // int_3.Allways2()はconstexprt定数
    static_assert(int_3.Allways3() == 3); // int_3はconstexprインスタンスではないが、
                                          // int_3.Allways3()はconstexprt定数
    // @@@ sample end
    // clang-format on
    IGNORE_UNUSED_VAR(i3);
}
// @@@ sample begin 4:0

constexpr Integer operator"" _i(unsigned long long int value)  // ユーザ定義リテラルの定義
{
    return Integer(static_cast<int32_t>(value));
}
// @@@ sample end

TEST(TermExp, user_defined_literal)
{
    // @@@ sample begin 4:1

    constexpr auto i = 123_i;
    static_assert(i == 123);
    static_assert(std::is_same_v<decltype(i), Integer const>);
    // @@@ sample end
}

// @@@ sample begin 5:0

constexpr uint64_t bit_mask(uint32_t max) { return max == 0 ? 0 : (1ULL << (max - 1)) | bit_mask(max - 1); }
constexpr uint64_t bit_mask_0 = bit_mask(4);  // C++11ではコンパイルエラー
static_assert(0b1111 == bit_mask_0);
// @@@ sample end
// @@@ sample begin 5:1

constexpr uint64_t bit_mask_for(uint32_t max)
{
    uint64_t ret = 0;

    for (auto i = 0u; i < max; ++i) {
        ret |= 1ULL << i;
    }

    return ret;
}
constexpr uint64_t bit_mask_1 = bit_mask_for(4);  // C++17からサポート
static_assert(0b1111 == bit_mask_1);
// @@@ sample end

template <uint32_t N>
struct bit_mask_struct {
    static constexpr uint64_t value = (1ULL << (N - 1)) | bit_mask_struct<N - 1>::value;
};

// 再帰の終了条件
template <>
struct bit_mask_struct<0> {
    static constexpr uint64_t value = 0;
};
static_assert(0b1111 == bit_mask_struct<4>::value);

namespace cpp20 {
// clang-format off
// @@@ sample begin 6:0

#if __cplusplus >= 202002L  // c++20
consteval uint64_t bit_mask(uint32_t max)  // コンパイル時、評価ができなければエラー

#else // C++17
constexpr uint64_t bit_mask(uint32_t max)  // コンパイル時、評価されるとは限らない
#endif
{
    if (max == 0) {
        return 0;
    }
    else {
        return (1ULL << (max - 1)) | bit_mask(max - 1);
    }
}
// @@@ sample end
// clang-format on

TEST(TermExp, bit_mask_recursive)
{
    // clang-format off
    // @@@ sample begin 6:1

    static_assert(0b1111'1111 == bit_mask(8));

    // auto i = 8UL;         // bit_maskがconstevalであるため、コンパイルエラー
    constexpr auto i = 8UL;  // iがconstexpであるためbit_maskががコンパイル時評価されるため、
    auto bm = bit_mask(i);   // bit_mask(i)の呼び出しは効率的になる
                             // bmをconsexprにするとさらに効率的になる

    ASSERT_EQ(0b1111'1111, bm);
    // @@@ sample end
    // clang-format on
}
}  // namespace cpp20

TEST(TermExp, constexpr_lmbda)
{
    // @@@ sample begin 7:0

    constexpr auto factorial = [](int n) {  // constexpr ラムダの定義
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    };

    constexpr int fact_5 = factorial(5);  // コンパイル時に計算される
    static_assert(fact_5 == 120);
    // @@@ sample end
}

TEST(TermExp, constexpr_lmbda2)
{
    // @@@ sample begin 7:1

    constexpr auto factorial = [](auto self, int n) -> int {  // リカーシブconstexprラムダ
        return (n <= 1) ? 1 : n * self(self, n - 1);
    };

    constexpr int fact_5 = factorial(factorial, 5);  // コンパイル時の評価
    static_assert(fact_5 == 120);
    // @@@ sample end
}
}  // namespace
