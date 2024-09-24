#pragma once

#include "nstd_concepts.h"
#include "nstd_type_traits.h"

// C : コンテナもしくはコンテナから派生した型
// T : コンテナの要素の型
// Ts: そのための型
// とした場合のC<T, Ts...>に対して、
//
// C<T, Ts...>が
// * Tがoperator<<を持つ
// * C<T, Ts...>に対してのstd::operator<<が存在しない
// * C<T, Ts...>がstd::cbegin, std::cendを持つ
// の条件を満たしたときのみ使用されるoperator<<を作りたい

// @@@ sample begin 0:0

namespace Nstd {
namespace Inner_ {

template <typename T>  // Nstd::Printableを使用するとg++のバグで、問題が発生するため、
                       // バグ回避のため敢えてここでNstd::Inner_::Printableを宣言する
// clang-format off
concept Printable = requires(T t, std::ostream& os)
{
    { os << t } -> std::same_as<std::ostream&>;
};
// clang-format on

template <typename T>
constexpr bool enable_range_put_to() noexcept
{
    if constexpr (Nstd::ValueType<T>::IsBuiltinArray) {  // Tは配列
        if constexpr (std::is_same_v<char,
                                     typename Nstd::ValueType<T>::type_direct>) {  // Tはchar配列
            return false;
        }
        else {
            return Nstd::Printable<typename Nstd::ValueTypeT<T>>;
        }
    }
    else {  // Tは配列ではない
        if constexpr (Printable<T>) {
            return false;
        }
        else {
            if constexpr (Nstd::IsRangeV<T>) {  // 範囲for文に適用できる
                return Nstd::Printable<typename Nstd::ValueTypeT<T>>;
            }
            else {
                return false;
            }
        }
    }
}

template <typename T>
constexpr bool enable_range_put_to_v{enable_range_put_to<T>()};
}  // namespace Inner_
}  // namespace Nstd
// @@@ sample end

// @@@ sample begin 1:0

namespace Nstd {
namespace Inner_ {

template <size_t N>
constexpr std::string_view range_put_to_sep() noexcept
{
    static_assert(N != 0);
    switch (N) {
    case 1:
        return ", ";
    case 2:
        return " | ";
    case 3:
    default:
        return " # ";
    }
};
}  // namespace Inner_

template <typename T>
auto operator<<(std::ostream& os, T const& t) ->
#if defined(__clang__)
    typename std::enable_if_t<Inner_::enable_range_put_to_v<T>, std::ostream&>
#else  // g++でのワークアラウンド
    typename std::enable_if_t<Inner_::enable_range_put_to<T>(), std::ostream&>
#endif
{
    auto sep = std::string_view("");
    auto s   = Inner_::range_put_to_sep<ValueType<T>::Nest>();

    for (auto const& i : t) {
        os << std::exchange(sep, s) << i;
    }

    return os;
}
}  // namespace Nstd
// @@@ sample end
