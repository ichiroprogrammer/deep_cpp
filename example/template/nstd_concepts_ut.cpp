#include <concepts>
#include <iterator>
#include <span>
#include <sstream>
#include <vector>

#include "gtest_wrapper.h"

#include "nstd_concepts.h"
#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace Nstd {
namespace {

TEST(Template, Array)
{
    // @@@ sample begin 0:0

    int  a[3];
    int* ptr = a;
    auto v   = std::vector{1, 2, 3};

    static_assert(Array<decltype(a)>);

    static_assert(Array<decltype(a)>);
    static_assert(!Array<decltype(ptr)>);
    // @@@ sample end
    IGNORE_UNUSED_VAR(ptr);
}

TEST(Template, Beginable)
{
    // @@@ sample begin 0:1

    int  a[3];
    int* ptr = a;
    auto v   = std::vector{1, 2, 3};

    static_assert(Array<decltype(a)>);

    static_assert(Beginable<decltype(a)>);
    static_assert(!Beginable<decltype(ptr)>);
    static_assert(Beginable<decltype(v)>);
    // @@@ sample end
    IGNORE_UNUSED_VAR(ptr);
}

namespace Container_NS {

TEST(Template, Ranged)
{
    // @@@ sample begin 0:2

    static_assert(Ranged<std::string>);
    static_assert(!Ranged<int>);
    static_assert(Ranged<int const[3]>);
    static_assert(Ranged<int[3]>);
    // @@@ sample end
}

// @@@ sample begin 0:3

struct X {
    std::vector<int> data{1, 2, 3, 4, 5};

    auto begin() { return data.begin(); }  // std::begin
    auto end() { return data.end(); }      // std::end
};
// @@@ sample end

TEST(Template, Container)
{
    // @@@ sample begin 0:4

    static_assert(Container<std::string>);
    static_assert(!Container<int>);
    static_assert(!Container<int const[3]>);

    static_assert(!Ranged<X>);  // begin/endがあるが、value_typeをもっていない

    // @@@ sample end
}
}  // namespace Container_NS

// @@@ sample begin 1:0

struct X {};  // Non-pritable
struct Y {};  // Printable

std::ostream& operator<<(std::ostream& os, Y)
{
    return os;  // 何もしない
}
// @@@ sample end

TEST(Template, Printable)
{
    // @@@ sample begin 1:1

    static_assert(Printable<bool>);
    static_assert(Printable<std::string>);
    static_assert(!Printable<std::vector<int>>);
    static_assert(Printable<std::vector<int>*>);
    static_assert(!Printable<X>);
    static_assert(Printable<Y>);
    // @@@ sample end

    Y                  y;
    std::ostringstream oss;

    oss << y;
    ASSERT_EQ("", oss.str());

    static_assert(Printable<decltype(y)>);
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
    static_assert(Nstd::ConvertibleToAll<int, int8_t, int16_t, int>);
    static_assert(Nstd::ConvertibleToAll<int, char, int, int>);
    static_assert(!Nstd::ConvertibleToAll<int, char*, int, int>);
    static_assert(Nstd::ConvertibleToAll<std::string, std::string, char*, char[3]>);
    static_assert(!Nstd::ConvertibleToAll<std::string, std::string, char*, int>);
}

TEST(Template, are_convertible_without_narrow_conv)
{
    static_assert(Nstd::Inner_::convertible_without_narrowing<int, int>);
    static_assert(Nstd::Inner_::convertible_without_narrowing<int, int16_t>);
    static_assert(!Nstd::Inner_::convertible_without_narrowing<int16_t, int>);
    static_assert(Nstd::Inner_::convertible_without_narrowing<std::string, char*>);
    static_assert(!Nstd::Inner_::convertible_without_narrowing<char*, std::string>);

    static_assert(Nstd::ConvertibleWithoutNarrowing<int, char, int16_t, uint16_t>);
    static_assert(!Nstd::ConvertibleWithoutNarrowing<int, char, int16_t, uint32_t>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<std::string, char[5], char*>);
    static_assert(Nstd::ConvertibleWithoutNarrowing<double, float>);

    // int8_t -> doubleは縮小型変換
    static_assert(!Nstd::ConvertibleWithoutNarrowing<double, float, int8_t>);

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

namespace {
template <class T, std::size_t Extent>
void print(std::span<T, Extent> s, std::ostream& os)
{
    const char* delimiter = "";

    os << '{';
    for (const T& x : s) {
        os << std::exchange(delimiter, ",") << x;
    }
    os << '}';
}

TEST(Template, span)
{
    std::vector<int> v{1, 2, 3, 4, 5};
    int              ar[]{1, 2, 3, 4, 5};

    {
        std::ostringstream oss;

        print(std::span{v}, oss);

        ASSERT_EQ("{1,2,3,4,5}", oss.str());
    }
    {
        std::ostringstream oss;

        print(std::span{ar}, oss);

        ASSERT_EQ("{1,2,3,4,5}", oss.str());
    }

    ASSERT_EQ("__gnu_cxx::__normal_iterator<int*, std::vector<int, std::allocator<int> > >",
              Nstd::Type2Str<decltype(std::begin(v))>());
    ASSERT_EQ("__gnu_cxx::__normal_iterator<int*, std::vector<int, std::allocator<int> > >",
              Nstd::Type2Str<decltype(std::end(v))>());

    ASSERT_EQ("int*", Nstd::Type2Str<decltype(std::begin(ar))>());
    ASSERT_EQ("int*", Nstd::Type2Str<decltype(std::end(ar))>());
}
}  // namespace
