#pragma once
#include <concepts>
#include <cstdint>
#include <exception>
#include <type_traits>

#include "nstd_static_string_num.h"

namespace Nstd {

// @@@ sample begin 0:0

/// @class Exception
/// @brief StaticString<>を使ったエクセプションクラス
///        下記のMAKE_EXCEPTIONを使い生成
/// @tparam E   std::exceptionから派生したエクセプションクラス
/// @tparam N   StaticString<N>
template <typename E, size_t N>
#if __cplusplus == 202002L  // c++20
requires std::derived_from<E, std::exception>
#endif
class Exception : public E {
public:
    static_assert(std::is_base_of_v<std::exception, E>);

    Exception(StaticString<N> const& what_str) noexcept : what_str_{what_str} {}
    char const* what() const noexcept override { return what_str_.String(); }

private:
    StaticString<N> const what_str_;
};
// @@@ sample end
// @@@ sample begin 0:1

namespace Inner_ {
template <typename E, template <size_t> class STATIC_STR, size_t N>
#if __cplusplus == 202002L  // c++20
requires std::derived_from<E, std::exception>
#endif
auto make_exception(STATIC_STR<N> exception_str) noexcept { return Exception<E, N>{exception_str}; }
}  // namespace Inner_

template <typename E, size_t LINE_NUM, size_t F_N, size_t M_N>
#if __cplusplus == 202002L  // c++20
requires std::derived_from<E, std::exception>
#endif
auto MakeException(char const (&filename)[F_N], char const (&msg)[M_N]) noexcept
{
    return Inner_::make_exception<E>(StaticString{filename} + ":" + Int2StaticString<LINE_NUM>()
                                     + ":" + msg);
}
// @@@ sample end
}  // namespace Nstd

// @@@ sample begin 0:2

#define MAKE_EXCEPTION(E__, msg__) Nstd::MakeException<E__, __LINE__>(__FILE__, msg__)
// @@@ sample end
