#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Rvalue {

void g0()
{
    // lvalueからの代入
    // @@@ sample begin 0:0

    auto str0 = std::string{};        // 行１   str0はlvalue
    auto str1 = std::string{"hehe"};  // 行２   str1もlvalue
    str0      = str1;                 // 行３   lvalueからの代入
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}

void g1()
{
    // clang-format off
    // rvalueからの代入
    // @@@ sample begin 0:1

    auto str0 = std::string{};        // 行１   str0はlvalue
    str0      = std::string{"hehe"};  // 行２   rvalueからの代入
                                      // 行３   行２で生成されたテンポラリオブジェクト(rvalue)は解放
    // @@@ sample end
    // clang-format on
    IGNORE_UNUSED_VAR(str0);
}

void g2()
{
    // std::move(lvalue)からの代入
    // @@@ sample begin 0:2

    auto str0 = std::string{};        // 行１   str0はlvalue
    auto str1 = std::string{"hehe"};  // 行２   str1もlvalue
    str0      = std::move(str1);      // 行３   str1への代入以外のアクセスは未規定である。
    // @@@ sample end
    IGNORE_UNUSED_VAR(str0);
}
}  // namespace Rvalue
