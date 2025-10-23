#include <memory>
#include <utility>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace utility {
// @@@ sample begin 0:0

uint32_t f(std::string&) { return 0; }         // f-0
uint32_t f(std::string&&) { return 1; }        // f-1
uint32_t f(std::string const&) { return 2; }   // f-2
uint32_t f(std::string const&&) { return 3; }  // f-3
// @@@ sample end

TEST(Semantics, move)
{
    // clang-format off
    // @@@ sample begin 0:1

    std::string       str{};
    std::string const cstr{};

    ASSERT_EQ(0, f(str));               // strはlvalue → f(std::string&)
    ASSERT_EQ(1, f(std::string{}));     // 一時オブジェクトはrvalue → f(std::string&&)
    ASSERT_EQ(1, f(std::move(str)));    // std::moveでrvalueに変換 → f(std::string&&)
    ASSERT_EQ(2, f(cstr));              // cstrはconst lvalue → f(std::string const&)
    ASSERT_EQ(3, f(std::move(cstr)));   // std::moveでconst rvalueに変換 → f(std::string const&&)
    // @@@ sample end
    // clang-format on

    ASSERT_EQ("std::string&&", Nstd::Type2Str<decltype(std::move(str))>());
    ASSERT_EQ("std::string const&&", Nstd::Type2Str<decltype(std::move(cstr))>());
}
}  // namespace utility
