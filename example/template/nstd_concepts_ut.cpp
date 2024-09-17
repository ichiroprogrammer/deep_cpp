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

    static_assert(Array<decltype(a)>);
    static_assert(Container<decltype(v)>);

    static_assert(Beginable<decltype(a)>);
    static_assert(!Beginable<decltype(ptr)>);
    static_assert(Beginable<decltype(v)>);

    static_assert(Endable<decltype(a)>);
    static_assert(!Endable<decltype(ptr)>);
    static_assert(Endable<decltype(v)>);

    // @@@ sample end
    IGNORE_UNUSED_VAR(ptr);
}

TEST(Template, Ranged)
{
    // @@@ sample begin 4:0

    static_assert(Ranged<std::string>);
    static_assert(!Ranged<int>);
    static_assert(Ranged<int const[3]>);
    static_assert(Ranged<int[3]>);
    // @@@ sample end
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

TEST(Template, OneOf)
{
    static_assert(!Nstd::OneOf<int, int8_t, int16_t, uint16_t>);
    static_assert(Nstd::OneOf<int, int8_t, int16_t, uint16_t, int32_t>);
    static_assert(Nstd::OneOf<int&, int8_t, int16_t, int32_t&, int32_t>);
    static_assert(!Nstd::OneOf<int&, int8_t, int16_t, uint32_t&, int32_t>);
    static_assert(Nstd::OneOf<std::string, int, char*, std::string>);
    static_assert(!Nstd::OneOf<std::string, int, char*>);
}

TEST(Template, are_convertible)
{
    // @@@ sample begin 1:0

    static_assert(Nstd::ConvertibleToAll<int, int8_t, int16_t, int>);
    static_assert(Nstd::ConvertibleToAll<int, char, int, int>);
    static_assert(!Nstd::ConvertibleToAll<int, char*, int, int>);
    static_assert(Nstd::ConvertibleToAll<std::string, std::string, char*, char[3]>);
    static_assert(!Nstd::ConvertibleToAll<std::string, std::string, char*, int>);
    // @@@ sample end
}

TEST(Template, are_convertible_without_narrow_conv)
{
    // @@@ sample begin 2:0

    static_assert(Nstd::Inner_::convertible_without_narrowing<int, int>);
    static_assert(Nstd::Inner_::convertible_without_narrowing<int, int16_t>);
    static_assert(!Nstd::Inner_::convertible_without_narrowing<int16_t, int>);
    static_assert(Nstd::Inner_::convertible_without_narrowing<std::string, char*>);
    static_assert(!Nstd::Inner_::convertible_without_narrowing<char*, std::string>);
    // @@@ sample end
    // @@@ sample begin 2:1

    static_assert(Nstd::ConvertibleWithoutNarrowing<int, char, int16_t, uint16_t>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<int, char, int16_t, uint32_t>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<std::string, char[5], char*>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<double, float>);

    // int8_t -> doubleは縮小型変換
    static_assert(!Nstd::ConvertibleWithoutNarrowing<double, float, int8_t>);
    // @@@ sample end

    static_assert(Nstd::ConvertibleWithoutNarrowing<int, int>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<int, int16_t>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<int, uint16_t>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<int, char>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<int, char, int16_t, uint16_t>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<int, void>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<void, void>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<int, uint32_t>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<double, float>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<double, int>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<double, float, int>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<float, double>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<float, int>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<std::string, char[5]>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<std::string, char[5], char*>);
}
}  // namespace
}  // namespace Nstd
