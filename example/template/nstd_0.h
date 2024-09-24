#pragma once
#include <vector>

// @@@ sample begin 0:0

namespace Nstd {

template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& vec)
{
    auto sep = "";

    for (auto const& i : vec) {
        os << std::exchange(sep, ", ") << i;
    }

    return os;
}

template <typename T>
std::string ToString(std::vector<T> const& vec)
{
    auto oss = std::ostringstream{};

    oss << vec;

    return oss.str();
}
}  // namespace Nstd
// @@@ sample end
