#include <concepts>
#include <ostream>
#include <type_traits>

#pragma once

namespace Nstd {

// @@@ sample begin 0:0

template <typename T>
concept Array = std::is_array_v<T>;

template <typename T>
concept Beginable = Array<T> || requires(T& t)
{
    {
        std::begin(t)
        } -> std::same_as<typename T::iterator>;
};

template <typename T>
concept Endable = Array<T> || requires(T& t)
{
    {
        std::end(t)
        } -> std::same_as<typename T::iterator>;
};

template <typename T>
concept Ranged = Beginable<T> && Endable<T>;
// @@@ sample end

template <typename T>
concept Container = Ranged<T> && !Array<T>;

// clang-format off
template <typename T>
concept Printable = requires(T t, std::ostream& os) // <- ExistsPutToV
{
    { os << t } -> std::same_as<std::ostream&>;
};
// clang-format on

template <typename T, typename... Us>
concept OneOf = (std::same_as<T, Us> || ...);  // <-IsSameSomeOfV

// 複数の型 FROMs がすべて TO に変換可能かどうかを制約するコンセプト
template <typename TO, typename... FROMs>
concept ConvertibleToAll = (std::convertible_to<FROMs, TO> && ...);  // <- AreConvertibleV

namespace Inner_ {

template <typename TO, typename FROM>
concept convertible_without_narrowing = requires(FROM from)
{
    // clang-format off
    { TO {from} } -> std::same_as<TO>;
    // clang-format on
};

// 複数の型が変換可能かどうかを確認するバージョン
template <typename TO, typename... FROMs>
concept all_convertible_without_narrowing = (convertible_without_narrowing<TO, FROMs> && ...);

}  // namespace Inner_

// コンセプトを使用
template <typename TO, typename... FROMs>
concept ConvertibleWithoutNarrowing = Inner_::all_convertible_without_narrowing<TO, FROMs...>;
// <-AreConvertibleWithoutNarrowConvV
}  // namespace Nstd
