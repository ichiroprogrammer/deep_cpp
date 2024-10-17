#include <vector>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {

TEST(TermExp, template_constant)
{
    // @@@ sample begin 0:0

    auto a = std::vector<int>{1, 2, 3};

    static_assert(std::is_same_v<decltype(a), std::vector<int>>);
    // @@@ sample end
}

TEST(TermExp, template_constant_new)
{
    // clang-format off
    // @@@ sample begin 0:1

    auto a = std::vector{1, 2, 3};

    static_assert(std::is_same_v<decltype(a), std::vector<int>>);  // テンプレート引数がintと推論
    // @@@ sample end
    // clang-format on
}
// @@@ sample begin 1:0

template <typename T>
struct is_void {
    static constexpr bool value = false;
};

template <>
struct is_void<void> {
    static constexpr bool value = true;
};

static_assert(is_void<void>::value);
static_assert(!is_void<int>::value);
// 以上はC++14以前のスタイル

// 以下はC++17から導入された
template <typename T>
constexpr bool is_void_v = is_void<T>::value;

static_assert(is_void_v<void>);
static_assert(!is_void_v<int>);
// @@@ sample end

// @@@ sample begin 2:0

using IntVector = std::vector<int>; // std::vector<int> のエイリアスを定義

template <typename T> //エイリアステンプレートを定義
using Vec = std::vector<T>;

static_assert(std::is_same_t<IntVector, Vec<int>>); // Vec<int> == std::vector<int>
// @@@ sample end
}// namespace
