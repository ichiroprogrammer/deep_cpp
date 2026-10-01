#include <sstream>

#include "gtest_wrapper.h"

// @@@ sample begin 0:0

// 二段階文字列化イデオムマクロ
#define STRINGIZE_INTERNAL(x) #x
#define STRINGIZE(x) STRINGIZE_INTERNAL(x)
// @@@ sample end
// @@@ sample begin 0:1

#define WHERE __FILE__ ":" STRINGIZE(__LINE__)
// @@@ sample end

TEST(Stringize, whare)
{
    // @@@ sample begin 0:2

    std::stringstream oss;
    auto              line_no = __LINE__ + 2;  // テスト対象の行は2行下

    char const whare[]{WHERE};
    oss << __FILE__ ":" << line_no;

    ASSERT_EQ(oss.str(), whare);
    // @@@ sample end
}
