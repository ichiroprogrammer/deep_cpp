#include <sstream>

#include "gtest_wrapper.h"

#include "stringize.h"

TEST(Stringize, whare)
{
    // @@@ sample begin 1:0

    std::stringstream oss;
    auto line_no = __LINE__ + 2;  // テスト対象のWHEREが書かれている行は、この行の2行下

    char const whare[]{WHERE};
    oss << __FILE__ ":" << line_no;

    ASSERT_EQ(oss.str(), whare);
    // @@@ sample end
}
