#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "gtest_wrapper.h"

namespace {

// @@@ sample begin 0:0

auto add(auto lhs, auto rhs)  // c++20で導入された記法
{
    return lhs + rhs;
}
// @@@ sample end

TEST(Decltype, eazy_template)
{
    // @@@ sample begin 0:1

    ASSERT_EQ(add(1, 2), 3);

    ASSERT_DOUBLE_EQ(add(1.5, 2.5), 4.0);

    using namespace std::literals::string_literals;

    ASSERT_EQ(add("hello"s, "world"s), "helloworld"s);
    // @@@ sample end
}
}  // namespace
