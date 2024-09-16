#include <concepts>
#include <sstream>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_concepts.h"
#include "nstd_type2str.h"
#include "suppress_warning.h"
namespace Nstd {
namespace {
TEST(Template, Beginable)
{
    // @@@ sample begin 0:0

    int  a[3];
    int* ptr = a;

    auto v = std::vector{1, 2, 3};

    static_assert(Beginable<decltype(a)>);
    static_assert(!Beginable<decltype(ptr)>);
    static_assert(Beginable<decltype(v)>);
    // @@@ sample end
    IGNORE_UNUSED_VAR(ptr);
}

struct X {};
struct Y {};

std::ostream& operator<<(std::ostream& os, Y)
{
    return os;  // 何もしない
}

TEST(Template, Printable)
{
    // @@@ sample begin 0:0

    int a{3};

    static_assert(Printable<decltype(a)>);
    static_assert(!Printable<X>);

    Y                  y;
    std::ostringstream oss;

    oss << y;
    ASSERT_EQ("", oss.str());

    static_assert(Printable<decltype(y)>);
    // @@@ sample end
}
}  // namespace
}  // namespace Nstd
