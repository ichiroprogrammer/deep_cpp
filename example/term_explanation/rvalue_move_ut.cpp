#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Rvalue {

void g0()
{
    // @@@ sample begin 0:0

    auto str0 = std::string{};        // str0はlvalue
    auto str1 = std::string{"hehe"};  // str1もlvalue
    str0      = str1;                 // lvalueからの代入
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}

void g1()
{
    // @@@ sample begin 0:1

    auto str0 = std::string{};        // str0はlvalue
    str0      = std::string{"hehe"};  // rvalueからの代入
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}

void g2()
{
    // @@@ sample begin 0:2

    auto str0 = std::string{};        // str0はlvalue
    auto str1 = std::string{"hehe"};  // str1もlvalue
    str0      = std::move(str1);      // str1はこれ以降使われないとする
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}
}  // namespace Rvalue
