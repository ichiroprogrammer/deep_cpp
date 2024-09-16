#include <concepts>
#include <ostream>

#pragma once

namespace Nstd {

// @@@ sample begin 0:0

template <typename T>
concept Beginable = requires(T& t)
{
    {std::begin(t)};  // std::beginが呼び出せることを制約
};
// @@@ sample end

template <typename T>
concept Printable = requires(T t, std::ostream& os)
{
    {
        os << t
        } -> std::same_as<std::ostream&>;
};

}  // namespace Nstd
