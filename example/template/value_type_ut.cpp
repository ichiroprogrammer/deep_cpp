#include <list>
#include <type_traits>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_concepts.h"
#include "nstd_type2str.h"
#include "nstd_type_traits.h"
#include "suppress_warning.h"

namespace {

namespace ValueTypeNotRecursive {

using namespace Nstd;
// リカーシブでないValueType
// @@@ sample begin 0:0

template <typename T, typename = void>
struct ValueType {
    using type = void;
};

template <typename T, size_t N>
struct ValueType<T[N]> {  // 配列型の特殊化
    using type = T;
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;
// @@@ sample end

TEST(Template, value_type_not_recursive)
{
    // @@@ sample begin 0:1

    static_assert(std::is_same_v<int, ValueTypeT<int[1]>>);
    static_assert(std::is_same_v<void, ValueTypeT<int>>);
    static_assert(std::is_same_v<int[2], ValueTypeT<int[1][2]>>);
    // @@@ sample end
}
}  // namespace ValueTypeNotRecursive

namespace ValueTypeNest {

using namespace Nstd;
// Nestのサポート
// @@@ sample begin 1:0

template <typename T, typename = void>
struct ValueType {
    using type        = void;
    using type_direct = T;
    static constexpr size_t Nest{0};
};

template <typename T, size_t N>
struct ValueType<T[N]> {  // 配列型の特殊化
    using type                     = T;
    using type_direct              = T;  // T = S[N]の場合、Tを保存
    static constexpr size_t extent = N;  // T = S[N]の場合、Nを保存
    static constexpr size_t Nest{ValueType<type>::Nest + 1};
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;
// @@@ sample end

TEST(Template, value_type_recursive)
{
    static_assert(std::is_same_v<int, ValueTypeT<int[1]>>);
    static_assert(std::is_same_v<void, ValueTypeT<int>>);
    static_assert(std::is_same_v<int[2], ValueTypeT<int[1][2]>>);

    // @@@ sample begin 1:1

    static_assert(0 == ValueType<int>::Nest);
    static_assert(1 == ValueType<int[1]>::Nest);
    static_assert(2 == ValueType<int[1][2]>::Nest);
    static_assert(1 == ValueType<int[1]>::extent);
    static_assert(1 == ValueType<int[1][2]>::extent);  // int[1][2] == (int[1])[2]
    // @@@ sample end

    static_assert(3 == ValueType<int[1][2][3]>::Nest);
}
}  // namespace ValueTypeNest

namespace ValueTypeN_NS {

// @@@ sample begin 2:0

template <typename T>
struct ValueType {
    using type                     = T;
    using type_direct              = T;
    static constexpr size_t extent = 0;
    static constexpr size_t Nest   = 0;

    template <size_t N>
    using type_n = std::conditional_t<N == 0, T, void>;  // Nが0のときはT、それ以外はvoidを返す
};
// @@@ sample end
// @@@ sample begin 2:1

template <typename T, size_t N>  // Array Tを使わずに配列の一般的な表現を使用してNを取り出す
struct ValueType<T[N]> {         // 配列型の特殊化
    using type                     = typename ValueType<T>::type;
    using type_direct              = T;
    static constexpr size_t extent = N;
    static constexpr size_t Nest   = ValueType<T>::Nest + 1;

    template <size_t M>
    using type_n = std::conditional_t<M == 0, T[N], typename ValueType<T>::template type_n<M - 1>>;
};
// @@@ sample end

template <typename T>
using ValueTypeT = typename ValueType<T>::type;

template <typename T, size_t N>
using ValueTypeT_n = typename ValueType<T>::template type_n<N>;

TEST(Template, type_n)
{
    // @@@ sample begin 2:3

    // ValueType<int[1][2][3]>の展開について、考えよう。

    // int[1][2][3]は == (int[2][3])[1]であるため、下記の式が成立する
    static_assert(std::is_same_v<ValueType<int[1][2][3]>::type_direct, int[2][3]>);
    static_assert(ValueType<int[1][2][3]>::extent == 1);

    // type_n<M>の仕様は、Mが1の時、配列から1階層を1枚剥ぎ取ることである
    using T1 = ValueType<int[1][2][3]>::type_n<1>;  // 長い式は可読性が劣化するからT1を宣言

    // int[2][3] == (int[3])[2]であるため、下記の式が成立する
    static_assert(std::is_same_v<T1, int[2][3]>);
    static_assert(ValueType<T1>::extent == 2);

    // 上記T1と同様にT2を宣言する
    using T2 = ValueType<T1>::type_n<1>;

    // int[2][3] == (int[3])[2]であるため、下記の式が成立する
    static_assert(std::is_same_v<T2, int[3]>);
    static_assert(ValueType<T2>::extent == 3);

    // 上記T1、T2と同様にT3を宣言する
    using T3 = ValueType<T2>::type_n<1>;

    static_assert(std::is_same_v<T3, int>);
    static_assert(ValueType<T3>::extent == 0);
    // @@@ sample end
    // clang-format off
    // @@@ sample begin 2:4

    using T = int[1][2][3];

    static_assert(std::is_same_v<int[1][2][3], ValueType<T>::type_n<0>>);  // 0枚剝く
    static_assert(std::is_same_v<int[2][3], ValueType<T>::type_n<1>>);     // 1枚剝く
    static_assert(std::is_same_v<int[3], ValueType<T>::type_n<2>>);        // 2枚剝く
    static_assert(std::is_same_v<int, ValueType<T>::type_n<3>>);           // 3枚剝く
    static_assert(std::is_same_v<void, ValueType<T>::type_n<4>>);          // 全部剝く
    // @@@ sample end
    // clang-format on
    {
        using T = int[1][2][3];

        static_assert(ValueType<T>::Nest == 3);
        static_assert(std::is_same_v<int[1][2][3], ValueType<T>::type_n<0>>);
        static_assert(std::is_same_v<int[2][3], ValueType<T>::type_n<1>>);
        static_assert(std::is_same_v<int[3], ValueType<T>::type_n<2>>);
        static_assert(std::is_same_v<int, ValueType<T>::type_n<3>>);
        static_assert(std::is_same_v<void, ValueType<T>::type_n<4>>);
        static_assert(std::is_same_v<void, ValueType<T>::type_n<5>>);
    }
    {
        // @@@ sample begin 2:5

        using T = ValueTypeT_n<int[1][2][3], 3>;

        static_assert(std::is_same_v<int, T>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, ValueType<T>::Nest>>);
        // @@@ sample end
    }
    {
        using T = int[1][2];

        static_assert(std::is_same_v<int[1][2], ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int[2], ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 5>>);
    }
    {
        using T = int[1];

        static_assert(std::is_same_v<int[1], ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<int, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 3>>);
    }
    {
        using T = int;

        static_assert(std::is_same_v<int, ValueTypeT_n<T, 0>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 1>>);
        static_assert(std::is_same_v<void, ValueTypeT_n<T, 2>>);
    }
}
}  // namespace ValueTypeN_NS

namespace ValueTypeN2_NS {

template <typename T>
constexpr size_t rank()
{
    if constexpr (std::is_array_v<T>) {
        return std::rank_v<T>;
    }

    if constexpr (Nstd::Container<T>) {
        return 1 + rank<typename T::value_type>();
    }

    return 0;
}

TEST(Template, rank)
{
    static_assert(rank<int>() == 0);
    static_assert(rank<int[1][2]>() == 2);

    using T = std::vector<std::vector<std::vector<int>>>;

    std::cout << "rank:" << rank<T>() << std::endl;
    static_assert(rank<std::vector<int>>() == 1);
}

// @@@ sample begin 3:0

template <typename T>
struct ValueType {  // プライマリ
    using type                   = T;
    static constexpr size_t Nest = 0;

    template <size_t N>
    using type_n = std::conditional_t<N == 0, T, void>;
};

template <typename T, size_t N>
struct ValueType<T[N]> {  // 配列型の特殊化
    using type                   = typename ValueType<T>::type;
    static constexpr size_t Nest = ValueType<T>::Nest + 1;

    template <size_t M>
    using type_n = std::conditional_t<M == 0, T[N], typename ValueType<T>::template type_n<M - 1>>;
};
// @@@ sample end
using Nstd::Container;
// @@@ sample begin 3:1

template <Container T>
struct ValueType<T> {  // コンテナ型の特殊化
    using type                   = typename ValueType<typename T::value_type>::type;
    static constexpr size_t Nest = ValueType<typename T::value_type>::Nest + 1;

    // clang-format off
    template <size_t N>
    using type_n = std::conditional_t<N == 0, T, typename ValueType<typename T::value_type>::template type_n<N - 1>>;
    // clang-format on
};
// @@@ sample end

TEST(Template, value_type_n_2)
{
    {
        // @@@ sample begin 3:2

        using T = int[1][2][3];

        // コンテナ特殊化以外の機能チェック
        static_assert(std::is_same_v<T, ValueType<T>::type_n<0>>);
        static_assert(std::is_same_v<int[2][3], ValueType<T>::type_n<1>>);
        static_assert(std::is_same_v<int[3], ValueType<T>::type_n<2>>);
        static_assert(std::is_same_v<int, ValueType<T>::type_n<3>>);
        static_assert(std::is_same_v<int, ValueType<T>::type>);
        // @@@ sample end
    }
    {
        // @@@ sample begin 3:3

        // コンテナ特殊化の機能の機能チェック
        using T = std::vector<std::vector<std::vector<int>>>;

        static_assert(std::is_same_v<T, ValueType<T>::type_n<0>>);
        static_assert(std::is_same_v<int, ValueType<T>::type_n<3>>);
        static_assert(ValueType<T>::Nest == 3);
        static_assert(std::is_same_v<int, ValueType<T>::type>);
        // @@@ sample end
    }
    {
        // @@@ sample begin 3:4

        // 統合した機能チェック
        using T = std::vector<int[1][2][3]>;

        static_assert(std::is_same_v<T, ValueType<T>::type_n<0>>);
        static_assert(std::is_same_v<int[1][2][3], ValueType<T>::type_n<1>>);
        static_assert(std::is_same_v<int[2][3], ValueType<T>::type_n<2>>);
        static_assert(std::is_same_v<int[3], ValueType<T>::type_n<3>>);
        static_assert(std::is_same_v<int, ValueType<T>::type_n<4>>);
        static_assert(std::is_same_v<int, ValueType<T>::type>);
        static_assert(ValueType<T>::Nest == 4);

        using L = std::list<T>;

        static_assert(std::is_same_v<T, ValueType<L>::type_n<1>>);
        static_assert(std::is_same_v<int[1][2][3], ValueType<L>::type_n<2>>);
        static_assert(std::is_same_v<int, ValueType<T>::type>);
        static_assert(ValueType<L>::Nest == 5);
        // @@@ sample end
    }
}
}  // namespace ValueTypeN2_NS
}  // namespace
