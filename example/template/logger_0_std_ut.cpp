#include <cassert>
#include <vector>

#include "gtest_wrapper.h"

#include "app_ints.h"
#include "logger_0.h"
#include "logger_ut.h"
#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace std {  // operator<<の定義をstdで行う
ostream& operator<<(std::ostream& os, App::Ints_t const& ints)
{
    auto sep = "";

    for (auto const& i : ints) {
        os << std::exchange(sep, ", ") << i;
    }

    return os;
}
}  // namespace std
// @@@ sample end

// clang-format off
#define INTS_UT_NAME logger_0_Ints_std
#include "logger_0_ints_ut.h"
// clang-format on
// 「operator<<の定義をstdで行う」が文字化けする
// 「operator<<の定義をstdで行う」が文字化けする
