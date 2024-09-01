#pragma once
#include <iterator>
#include <type_traits>
#include <utility>

namespace Nstd {

// @@@ sample begin 0:0

template <size_t N>
class StaticString {
public:
    constexpr StaticString(char const (&str)[N]) noexcept
        : StaticString{0, str, std::make_index_sequence<N - 1>{}}
    {
    }

    template <size_t M>
    constexpr StaticString(size_t offset, StaticString<M> ss) noexcept
        : StaticString{offset, ss.string_, std::make_index_sequence<N - 1>{}}
    {
    }

    constexpr StaticString(std::initializer_list<char> args) noexcept
        : StaticString{0, args, std::make_index_sequence<N - 1>{}}
    {
    }

    constexpr char const (&String() const noexcept)[N] { return string_; }
    constexpr size_t Size() const noexcept { return N; }

private:
    char const string_[N];

    template <typename T, size_t... I>
    // offsetは部分StaticString切り出しのため(TopStr, BottomStr)
    constexpr StaticString(size_t offset, T& t, std::index_sequence<I...>) noexcept
        : string_{std::begin(t)[I + offset]...}
    {
    }

    template <size_t M>
    friend class StaticString;
};
// @@@ sample end
// @@@ sample begin 0:1

namespace Inner_ {
template <size_t N>
constexpr bool equal_n(size_t n, StaticString<N> const& lhs, StaticString<N> const& rhs) noexcept
{
    if (n == N) {
        return true;
    }
    else {
        return lhs.String()[n] != rhs.String()[n] ? false : equal_n(n + 1, lhs, rhs);
    }
}
}  // namespace Inner_

template <size_t N1, size_t N2>
constexpr bool operator==(StaticString<N1> const&, StaticString<N2> const&) noexcept
{
    return false;
}

template <size_t N1, size_t N2>
constexpr bool operator!=(StaticString<N1> const& lhs, StaticString<N2> const& rhs) noexcept
{
    return !(lhs == rhs);
}

template <size_t N>
constexpr bool operator==(StaticString<N> const& lhs, StaticString<N> const& rhs) noexcept
{
    return Inner_::equal_n(0, lhs, rhs);
}

template <size_t N>
constexpr bool operator!=(StaticString<N> const& lhs, StaticString<N> const& rhs) noexcept
{
    return !(lhs == rhs);
}

template <size_t N1, size_t N2>
constexpr bool operator==(StaticString<N1> const& lhs, char const (&rhs)[N2]) noexcept
{
    return lhs == StaticString{rhs};
}

template <size_t N1, size_t N2>
constexpr bool operator!=(StaticString<N1> const& lhs, char const (&rhs)[N2]) noexcept
{
    return !(lhs == rhs);
}

template <size_t N1, size_t N2>
constexpr bool operator==(char const (&lhs)[N1], StaticString<N2> const& rhs) noexcept
{
    return StaticString{lhs} == rhs;
}

template <size_t N1, size_t N2>
constexpr bool operator!=(char const (&lhs)[N1], StaticString<N2> const& rhs) noexcept
{
    return !(lhs == rhs);
}
// @@@ sample end
// @@@ sample begin 0:2

namespace Inner_ {
template <size_t N1, size_t... I1, size_t N2, size_t... I2>
constexpr StaticString<N1 + N2 - 1> concat(char const (&str1)[N1], std::index_sequence<I1...>,
                                           char const (&str2)[N2],
                                           std::index_sequence<I2...>) noexcept
{
    return {str1[I1]..., str2[I2]...};
}
}  // namespace Inner_

template <size_t N1, size_t N2>
constexpr auto operator+(StaticString<N1> const& lhs, StaticString<N2> const& rhs) noexcept
{
    return Inner_::concat(lhs.String(), std::make_index_sequence<N1 - 1>{}, rhs.String(),
                          std::make_index_sequence<N2>{});
}

template <size_t N1, size_t N2>
constexpr auto operator+(StaticString<N1> const& lhs, char const (&rhs)[N2]) noexcept
{
    return lhs + StaticString{rhs};
}

template <size_t N1, size_t N2>
constexpr auto operator+(char const (&lhs)[N1], StaticString<N2> const& rhs) noexcept
{
    return StaticString{lhs} + rhs;
}
// @@@ sample end
// @@@ sample begin 0:3

template <size_t SIZE, size_t N>  // StaticString<SiZE>の部分文字列生成
constexpr auto TopStr(StaticString<N> ss) noexcept
{
    return StaticString<SIZE + 1>{0, ss};  // SIZE文字 + 終端文字
}

template <size_t OFFSET, size_t N>  // 先頭からオフセット2文字～終端文字まで
constexpr auto BottomStr(StaticString<N> ss) noexcept
{
    return StaticString<N - OFFSET>{OFFSET, ss};
}
// @@@ sample end
}  // namespace Nstd
