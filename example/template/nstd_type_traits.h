#pragma once
#include <concepts>
#include <ostream>
#include <type_traits>

#include "nstd_concepts.h"

// namespaceを細かく刻んでいるのは、mdファイルの構成の都合

//
// IsSameSomeOf
//
// @@@ sample begin 0:0

namespace Nstd {
// コンセプト: 複数の型のいずれかがTと同じかどうかをチェック
template <typename T, typename U, typename... Us>
concept SameAsSomeOf = (std::same_as<T, U> || (std::same_as<T, Us> || ...));

// 型特性: TがUsのいずれかと同じ場合true_type、そうでない場合false_typeを継承
template <typename T, typename U, typename... Us>
struct IsSameSomeOf : std::bool_constant<SameAsSomeOf<T, U, Us...>> {
};

// 便利な定数テンプレート
template <typename T, typename U, typename... Us>
constexpr bool IsSameSomeOfV = IsSameSomeOf<T, U, Us...>::value;
}  // namespace Nstd
// @@@ sample end
// replace IsSameSomeOfV -> OneOf
//
// AreConvertible
//
// @@@ sample begin 1:0

namespace Nstd {
namespace Inner_ {

template <typename TO, typename FROM, typename... FROMs>
struct are_convertible {
    static constexpr bool value{
        std::is_convertible_v<FROM, TO> && are_convertible<TO, FROMs...>::value};
};

template <typename TO, typename FROM>
struct are_convertible<TO, FROM> {
    static constexpr bool value{std::is_convertible_v<FROM, TO>};
};

template <typename TO, typename... FROMs>
constexpr bool are_convertible_v{are_convertible<TO, FROMs...>::value};
}  // namespace Inner_

template <typename TO, typename... FROMs>
struct AreConvertible
    : std::conditional_t<Inner_::are_convertible_v<TO, FROMs...>, std::true_type, std::false_type> {
};

template <typename TO, typename... FROMs>
constexpr bool AreConvertibleV{AreConvertible<TO, FROMs...>::value};
}  // namespace Nstd
// @@@ sample end
// replace AreConvertibleV -> ConvertibleToAll

//
// AreConvertibleWithoutNarrowConv
//
// @@@ sample begin 2:0

namespace Nstd {
namespace Inner_ {

template <typename TO, typename FROM>
class is_convertible_without_narrow_conv {
    template <typename T = TO, typename U = FROM>
    static constexpr auto detector(T* t, U* u) noexcept
        // @@@ sample end
        // @@@ sample begin 2:1
        // 縮小無しでFROMからTOへ変換可能な場合、*t = T{*u}はwell-formed
        // 上記ではない場合、*t = T{*u}はill-formed
        -> decltype(*t = T{*u}, bool{})
    // @@@ sample end
    // @@@ sample begin 2:2
    {
        return true;
    }

    static constexpr bool detector(...) noexcept { return false; }

public:
    static constexpr bool value{is_convertible_without_narrow_conv::detector(
        static_cast<TO*>(nullptr), static_cast<FROM*>(nullptr))};
};

template <typename TO, typename FROM>
constexpr bool is_convertible_without_narrow_conv_v{
    is_convertible_without_narrow_conv<TO, FROM>::value};
}  // namespace Inner_
}  // namespace Nstd
// @@@ sample end
// @@@ sample begin 2:3

namespace Nstd {
namespace Inner_ {

// clang-format off
template <typename TO, typename FROM, typename... FROMs>
struct are_convertible_without_narrow_conv {
    static constexpr bool value{
        is_convertible_without_narrow_conv_v<TO, FROM> 
              && are_convertible_without_narrow_conv<TO, FROMs...>::value};
};
// clang-format on

template <typename TO, typename FROM>
struct are_convertible_without_narrow_conv<TO, FROM> {
    static constexpr bool value{is_convertible_without_narrow_conv_v<TO, FROM>};
};

template <typename TO, typename FROM, typename... FROMs>
constexpr bool are_convertible_without_narrow_conv_v{
    are_convertible_without_narrow_conv<TO, FROM, FROMs...>::value};
}  // namespace Inner_

template <typename TO, typename FROM, typename... FROMs>
struct AreConvertibleWithoutNarrowConv
    : std::conditional_t<Inner_::are_convertible_without_narrow_conv_v<TO, FROM, FROMs...>,
                         std::true_type, std::false_type> {
};

template <typename TO, typename FROM, typename... FROMs>
constexpr bool AreConvertibleWithoutNarrowConvV{
    AreConvertibleWithoutNarrowConv<TO, FROM, FROMs...>::value};
}  // namespace Nstd
// @@@ sample end
// AreConvertibleWithoutNarrowConv -> ConvertibleWithoutNarrowing

//
// IsRange
//
namespace Nstd {
namespace Inner_ {

// @@@ sample begin 3:0

template <typename, typename = void>
struct exists_begin : std::false_type {
};

template <typename T>
struct exists_begin<T, std::void_t<decltype(std::begin(std::declval<T&>()))>> : std::true_type {
};

template <typename T>
constexpr bool exists_begin_v{exists_begin<T>::value};
// @@@ sample end
// @@@ sample begin 3:1

template <typename, typename = void>
struct exists_end : std::false_type {
};

template <typename T>
struct exists_end<T, std::void_t<decltype(std::end(std::declval<T&>()))>> : std::true_type {
};
template <typename T>
constexpr bool exists_end_v{exists_end<T>::value};
// @@@ sample end
}  // namespace Inner_

// @@@ sample begin 3:2

template <typename T>
struct IsRange : std::conditional_t<Inner_::exists_begin_v<T> && Inner_::exists_end_v<T>,
                                    std::true_type, std::false_type> {
};

template <typename T>
constexpr bool IsRangeV{IsRange<T>::value};
// @@@ sample end
}  // namespace Nstd

//
// ExistsPutTo
//
// @@@ sample begin 4:0

namespace Nstd {

template <typename, typename = std::ostream&>
struct ExistsPutTo : std::false_type {
};

template <typename T>
struct ExistsPutTo<T, decltype(std::declval<std::ostream&>() << std::declval<T>())>
    : std::true_type {
};

template <typename T>
constexpr bool ExistsPutToV{ExistsPutTo<T>::value};
}  // namespace Nstd
// @@@ sample end

//
// ValueType
//
// @@@ sample begin 5:0

namespace Nstd {

template <typename T, typename = void>  // ValueTypeのプライマリ
struct ValueType {
    using type_direct = void;

    static constexpr bool   IsBuiltinArray{false};
    static constexpr size_t Nest{0};

    template <size_t N>
    using type_n = typename std::conditional_t<N == 0, T, void>;

    using type = type_n<Nest>;
};

namespace Inner_ {

#if 0  // C++17スタイル
template <typename T, size_t N>
struct conditional_value_type_n {
    using type = typename std::conditional_t<
        ValueType<T>::Nest != 0,
        typename ValueType<typename ValueType<T>::type_direct>::template type_n<N - 1>, T>;
};

template <typename T>
struct conditional_value_type_n<T, 0> {
    using type = T;
};
#else  // C++20スタイル

template <typename T>
concept NonZeroNest = ValueType<T>::Nest != 0;

template <NonZeroNest T, size_t N>
struct conditional_value_type_n {
    using type = typename ValueType<typename ValueType<T>::type_direct>::template type_n<N - 1>;
};

template <typename T>  // コンセプトの効果でSFINAEの回避
struct conditional_value_type_n<T, 0> {
    using type = T;
};
#endif

// エイリアステンプレート
template <typename T, size_t N>
using ConditionalValueTypeT_n = typename conditional_value_type_n<T, N>::type;

template <typename T, typename = void>
struct array_or_container : std::false_type {
};

template <Array T>
struct array_or_container<T> : std::true_type {
    using type = typename std::remove_extent_t<T>;
};

template <Container T>
struct array_or_container<T> : std::true_type {
    using type = typename T::value_type;
};

template <typename T>
constexpr bool array_or_container_v{array_or_container<T>::value};

template <typename T>
concept ArrayOrContainer = array_or_container_v<T>;
}  // namespace Inner_

#if 0  // C++17スタイル
template <typename T>       // ValueTypeの特殊化
struct ValueType<T, typename std::enable_if_t<Inner_::array_or_container_v<T>>> {
#else  // C++20スタイル
template <Inner_::ArrayOrContainer T>  // ValueTypeの特殊化
struct ValueType<T> {                  // コンセプトによるSFINAEの回避
#endif

// clang-format off
    using type_direct = typename Inner_::array_or_container<T>::type;

    static constexpr bool   IsBuiltinArray{std::is_array_v<T>};
    static constexpr size_t Nest{ValueType<type_direct>::Nest + 1};

    template <size_t N>
    using type_n = typename Inner_::conditional_value_type_n<T, N>::type;

    using type = type_n<Nest>;
// clang-format on
};

template <typename T>
using ValueTypeT = typename ValueType<T>::type;

template <typename T, size_t N>
using ValueTypeT_n = typename ValueType<T>::template type_n<N>;
}  // namespace Nstd
// @@@ sample end
