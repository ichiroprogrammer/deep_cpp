#include <concepts>
#include <ostream>
#include <type_traits>

#pragma once

namespace Nstd {
// @@@ sample begin 0:0

template <typename T>
concept Array = std::is_array_v<T>;
// @@@ sample end
// @@@ sample begin 0:1

// clang-format off
template <typename T>
concept Beginable = Array<T> || requires(T& t)
{
    { std::begin(t) } -> std::same_as<typename T::iterator>;
};
// clang-format on

// clang-format off
template <typename T>
concept Endable = Array<T> || requires(T& t)
{
    { std::end(t) } -> std::same_as<typename T::iterator>;
};
// clang-format on
// @@@ sample end
// @@@ sample begin 1:0

template <typename T>
concept Ranged = Beginable<T> && Endable<T>;
// @@@ sample end

// @@@ sample begin 1:0

template <typename T>
concept Container = (Ranged<T> && !Array<T>) || requires
{
    T::value_type;
};
// @@@ sample end

// @@@ sample begin 2:0

// clang-format off
template <typename T>
concept Printable = requires(T t, std::ostream& os)
{
    { os << t } -> std::same_as<std::ostream&>;
};
// @@@ sample end
// clang-format on
// <- ExistsPutToV

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
