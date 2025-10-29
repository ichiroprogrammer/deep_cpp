#include <tuple>
#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

namespace cpp14_style {  // c++14までのスタイル
template <typename T>
constexpr bool all_true(T arg)
{
    return arg;
}
template <typename T, typename... Args>
constexpr bool all_true(T arg, Args... args)
{
    return arg && all_true(args...);
}
}  // namespace cpp14_style

namespace cpp17_style {  // 畳み込み式を使用したスタイル
template <typename... Ts>
constexpr bool all_true(Ts... args)
{
    return (args && ...);  // 単項右畳み込み
}
}  // namespace cpp17_style

static_assert(cpp14_style::all_true(true, true, true));
static_assert(cpp17_style::all_true(true, true, true));
// @@@ sample end

// @@@ sample begin 0:1
namespace cpp14_style {  // c++14までのスタイル
template <typename T>
constexpr bool any_true(T arg)
{
    return arg;
}
template <typename T, typename... Args>
constexpr bool any_true(T arg, Args... args)
{
    return arg || any_true(args...);
}
}  // namespace cpp14_style

namespace cpp17_style {  // 畳み込み式を使用したスタイル
template <typename... Ts>
constexpr bool any_true(Ts... args)
{
    return (... || args);  // 単項左畳み込み
}
}  // namespace cpp17_style
static_assert(cpp14_style::any_true(false, false, true));
static_assert(cpp17_style::any_true(false, false, true));
// @@@ sample end

// @@@ sample begin 0:2

namespace cpp14_style {  // c++14までのスタイル
template <typename T>
constexpr int sum(T arg)
{
    return arg;
}
template <typename T, typename... Args>
constexpr int sum(T arg, Args... args)
{
    return arg + sum(args...);
}

}  // namespace cpp14_style

namespace cpp17_style {  // 畳み込み式を使用したスタイル
template <typename... Ts>
constexpr int sum(Ts... args)
{
    return (args + ... + 0);  // 二項右畳み込み (初期値: 0)
}
}  // namespace cpp17_style

static_assert(cpp14_style::sum(1, 2, 3));
static_assert(cpp17_style::sum(1, 2, 3));
// @@@ sample end

// @@@ sample begin 0:3

namespace cpp14_style {  // c++14までのスタイル
template <typename T>
constexpr int product(T arg)
{
    return arg;
}
template <typename T, typename... Args>
constexpr int product(T arg, Args... args)
{
    return arg * product(args...);
}
}  // namespace cpp14_style

namespace cpp17_style {  // 畳み込み式を使用したスタイル
template <typename... Ts>
constexpr int product(Ts... args)
{
    return (1 * ... * args);  // 二項左畳み込み (初期値: 1)
}
}  // namespace cpp17_style

static_assert(cpp14_style::product(2, 3, 4));
static_assert(cpp17_style::product(2, 3, 4));
// @@@ sample end

namespace cpp14 {
// @@@ sample begin 1:0
template <typename T, typename U, typename... Us>
struct is_same_some_of {
    static constexpr bool value{std::is_same_v<T, U> ? true : is_same_some_of<T, Us...>::value};
};

template <typename T, typename U>
struct is_same_some_of<T, U> {
    static constexpr bool value{std::is_same_v<T, U>};
};
// @@@ sample end
// @@@ sample begin 1:1

static_assert(is_same_some_of<int, int, double, char>::value);
static_assert(!is_same_some_of<int, double, char>::value);
static_assert(is_same_some_of<std::string, std::string, int>::value);
// @@@ sample end

TEST(cpp17, fold_expression) {}
}  // namespace cpp14

namespace cpp17 {

// @@@ sample begin 2:0
template <typename T, typename U, typename... Us>
struct is_same_some_of {
    static constexpr bool value = (std::is_same_v<T, U> || ... || std::is_same_v<T, Us>);
};
// @@@ sample end
// @@@ sample begin 2:1

static_assert(is_same_some_of<int, int, double, char>::value);
static_assert(!is_same_some_of<int, double, char>::value);
static_assert(is_same_some_of<std::string, std::string, int>::value);
// @@@ sample end

TEST(cpp17, fold_expression) {}
}  // namespace cpp17

}  // namespace
