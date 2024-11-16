#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "gtest_wrapper.h"

namespace DecltypeSample {
namespace {
TEST(Decltype, aut_decltype)
{
    // @@@ sample begin 0:0

    int32_t  x{3};
    int32_t& r{x};

    auto        a = r;  // aの型はint32_t
    decltype(r) b = r;  // bの型はint32_t&

    // std::is_sameはオペランドの型が同じか否かを返すメタ関数
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    // @@@ sample end
    ASSERT_EQ(3, a);
}

// @@@ sample begin 0:1

//  本来ならばA::dataは、
//      * A::Aでメモリ割り当て
//      * A::~Aでメモリ解放
//  すべきだが、何らかの理由でそれが出来ないとする
struct A {
    size_t   len;
    uint8_t* data;
};

void do_something(size_t len)
{
    auto deallocate = [](A* p) {
        delete[](p->data);
        delete p;
    };

    auto a_ptr = std::unique_ptr<A, decltype(deallocate)>{new A, deallocate};

    a_ptr->len  = len;
    a_ptr->data = new uint8_t[10];

    // @@@ ignore begin
    // @@@ ignore end
    // do something for a_ptr
    // @@@ ignore begin
    // @@@ ignore end

    // a_ptrによるメモリの自動解放
}
// @@@ sample end

TEST(Decltype, aut_decltype2)
{
    // @@@ sample begin 0:2

    int32_t  x{3};
    int32_t& r{x};

    auto           a = r;  // aの型はint32_t
    decltype(r)    b = r;  // bの型はint32_t&
    decltype(auto) c = r;  // cの型はint32_t&   C++14からサポート
                           // decltype(auto)は、decltypeに右辺の式を与えるための構文

    // std::is_sameはオペランドの型が同じか否かを返すメタ関数
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), int&>);
    static_assert(std::is_same_v<decltype(c), int&>);
    // @@@ sample end
    ASSERT_EQ(3, a);
}
}  // namespace
namespace func_return_type_decucation {
// @@@ sample begin 1:0

template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b)
{
    return a + b;
}

static_assert(std::is_same_v<decltype(add(1, 2)), int>);  // addの戻り値型はintに型推論
static_assert(std::is_same_v<decltype(add(1u, 2u)), uint32_t>);  // addの戻り値型はintに型推論
static_assert(std::is_same_v<decltype(add(std::string{"str"}, "2")),
                             std::string>);  // addの戻り値型はstd::stringに型推論
// @@@ sample end
}  // namespace func_return_type_decucation
namespace not_usefunc_return_type_decucation {
// @@@ sample begin 1:1

template <typename T, typename U>  // 戻り値型を後置する関数宣言
decltype(std::declval<T>() + std::declval<T>()) add(T a, U b)
{
    return a + b;
}

static_assert(std::is_same_v<decltype(add(1, 2)), int>);  // addの戻り値型はintに型推論
static_assert(std::is_same_v<decltype(add(1u, 2u)), uint32_t>);  // addの戻り値型はintに型推論
static_assert(std::is_same_v<decltype(add(std::string{"str"}, "2")),
                             std::string>);  // addの戻り値型はstd::stringに型推論
// @@@ sample end
}  // namespace not_usefunc_return_type_decucation
namespace {
TEST(Decltype, decltype_unique_ptr) { do_something(10); }

// @@@ sample begin 2:0

// 戻り値型autoが使えないと下記のような宣言が必要
// std::vector<std::string> split(std::string_view str, char delimiter)
auto split(std::string_view str, char delimiter)
{
    std::vector<std::string> result;
    std::string              token;

    for (char ch : str) {
        if (ch == delimiter) {
            if (!token.empty()) {
                result.emplace_back(std::move(token));
            }
        }
        else {
            token += ch;
        }
    }

    if (!token.empty()) {
        result.emplace_back(std::move(token));
    }

    return result;
}
// @@@ sample end

TEST(Decltype, return_auto)
{
    // @@@ sample begin 2:1

    auto result = split("hello,world", ',');

    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "hello");
    ASSERT_EQ(result[1], "world");
    // @@@ sample end
}

// @@@ sample begin 3:0

int16_t gvalue = 1;

auto getValue(int16_t a) -> auto& { return gvalue += a; }
// @@@ sample end

TEST(Decltype, return_auto2)
{
    // @@@ sample begin 3:1

    auto           ret1 = getValue(10);
    decltype(auto) ret2 = getValue(0);

    ASSERT_EQ(ret1, 11);
    ASSERT_EQ(ret2, 11);

    ASSERT_EQ(gvalue, 11);
    ret1 += 1;
    ASSERT_EQ(gvalue, 11);

    ret2 += 1;
    ASSERT_EQ(gvalue, 12);
    // @@@ sample end
    ASSERT_EQ(ret1, 12);
}

// clang-format off
// @@@ sample begin 4:0

#if __cplusplus >= 202002L  // c++20
auto add(auto lhs, auto rhs) { 
    return lhs + rhs; 
}

#else  // c++17
template <typename T, typename U>
auto add(T lhs, U rhs)
{
    return lhs + rhs;
}
#endif
// @@@ sample end
// clang-format on

TEST(Decltype, eazy_template)
{
    // @@@ sample begin 4:1

    ASSERT_EQ(add(1, 2), 3);

    ASSERT_DOUBLE_EQ(add(1.5, 2.5), 4.0);

    using namespace std::literals::string_literals;

    ASSERT_EQ(add("hello"s, "world"s), "helloworld"s);
    // @@@ sample end
}

}  // namespace
}  // namespace DecltypeSample
