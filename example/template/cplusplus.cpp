#include <iostream>
#include <type_traits>

// @@@ sample begin 0:0

#if __cplusplus >= 202002L  // c++20

// c++20以上の機能を使い、c++17以下の機能ではill-formedとなるコード
template <typename T, typename U>
concept same_as = requires(T const* t, U const* u)
{
    {t = u, u = t};
};
#else  // c++17以下の機能を使い上記のコードと同じ機能を持つ実装

template <typename T, typename U>
inline constexpr bool same_as = std::is_same_t<T, U>;
#endif
// @@@ sample end

static_assert(!same_as<int, void>);
static_assert(same_as<int, int>);
static_assert(!same_as<int, uint32_t>);
static_assert(same_as<std::string, std::basic_string<char>>);
