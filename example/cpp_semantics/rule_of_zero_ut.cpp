#include <list>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

class RuleZero {
public:
    RuleZero(std::list<std::string> const& strs, std::string const& s) : strs_{strs}, s_{s} {}
    std::list<std::string> const& GetStrs() const noexcept { return strs_; }
    std::string const&            GetStr() const noexcept { return s_; }

    // 特殊メンバ関数は、メンバの特殊メンバ関数に任せる
private:
    std::list<std::string> strs_{};
    std::string            s_{};
};
// @@@ sample end

TEST(Template, rule_of_zero)
{
    // @@@ sample begin 0:1

    auto z = RuleZero(std::list<std::string>{"1", "2", "3"}, "str");

    auto coied = z;             // コピーは自動生成に任せる(ゼロの原則)
    auto moved = std::move(z);  // ムーブも自動生成に任せる(ゼロの原則)

    ASSERT_EQ(coied.GetStr(), moved.GetStr());
    ASSERT_EQ(coied.GetStrs(), moved.GetStrs());
    // @@@ sample end
}
}  // namespace
