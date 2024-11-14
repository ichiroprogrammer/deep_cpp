#include <compare>

#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {
// @@@ sample begin 0:0

struct DoubleName {
    std::string name0;
    std::string name1;
    friend bool operator==(DoubleName const& lhs, DoubleName const& rhs) noexcept = default;
};

inline auto operator<=>(DoubleName const& lhs, DoubleName const& rhs) noexcept
{
    // name0 を比較し、等しくなければその比較結果を返す
    if (auto cmp = lhs.name0 <=> rhs.name0; cmp != 0) {
        return cmp;
    }

    return lhs.name1 <=> rhs.name1;  // name0が等しければ name1を比較
}
// @@@ sample end

TEST(TermExp, if_switch_init)
{
    DoubleName taro_hanako{"taro", "hanako"};
    DoubleName kotaro_hanako{"kotaro", "hanako"};

    ASSERT_EQ(taro_hanako, taro_hanako);
    ASSERT_GE(taro_hanako, kotaro_hanako);
}
}  // namespace
