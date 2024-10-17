#include <stdexcept>
#include <type_traits>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

std::string f_noexcept() noexcept  // エクセプションを発生させない
{
    return "No exceptions here!";
}

std::string f_except() noexcept(false)  // エクセプションを発生させる
{
    throw std::runtime_error{"always throw"};

    return "No exceptions here!";
}

// noexcept or noexcept(false)と宣言しない限りnoexceptでない
std::string f_except2()  // エクセプションを発生させる
{
    throw std::runtime_error{"always throw"};

    return "No exceptions here!";
}
// @@@ sample end

TEST(TermExp, use_noexcept)
{
    // clang-format off
    // @@@ sample begin 0:1

    static_assert(noexcept(f_noexcept()));  // エクセプションを発生させる可能性の確認
    static_assert(!noexcept(f_except()));   // エクセプションを発生させない可能性の確認
    static_assert(!noexcept(f_except2()));  // エクセプションを発生させない可能性の確認

    ASSERT_EQ(f_noexcept(), "No exceptions here!");  // 動作確認
    ASSERT_THROW(f_except(), std::runtime_error);    // エクセプションの発生確認
    ASSERT_THROW(f_except2(), std::runtime_error);   // エクセプションの発生確認
    // @@@ sample end
    // clang-format on
}

// @@@ sample begin 1:0

class PossiblyThrow {  // オブジェクト生成でエクセプションの発生可能性あり
public:
    PossiblyThrow() {}
};

// テンプレート型Tがnoexceptで生成可能なら、関数もnoexceptにする
template <typename T>
void t_f(T const&) noexcept(std::is_nothrow_constructible_v<T>)
{
    // Tを生成して、何らかの処理を行う
}
// @@@ sample end

TEST(TemplateFunctionTest, use_noexcept2)
{
    // @@@ sample begin 1:1

    auto i = int{};
    auto p = PossiblyThrow{};

    static_assert(!std::is_nothrow_constructible_v<PossiblyThrow>);
    static_assert(std::is_nothrow_constructible_v<decltype(i)>);
    static_assert(noexcept(t_f(i)));
    static_assert(!noexcept(t_f(p)));
    // @@@ sample end
}
}  // namespace
