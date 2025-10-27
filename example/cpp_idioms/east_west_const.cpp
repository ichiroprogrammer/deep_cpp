#include <mutex>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

TEST(coding_style, east_const)
{
    // @@@ sample begin 0:0

    char              str[] = "hehe";  // 配列strに書き込み可能
    char const*       str0  = str;  // str0が指すオブジェクトはconstなので、*str0への書き込み不可
    char* const       str1  = str;  // str1がconstなので、str1への代入不可
    char const* const str2  = str;  // *str2への書き込み不可、str2への代入不可

    auto lamda = [](char const(&str_ref)[5]) {  // str_refは配列へのconstリファレンス
        int ret = 0;

        for (char const& a : str_ref) {  // aはchar constリファレンス
            ret += a;
        }
        return ret;
    };
    // @@@ sample end

    ASSERT_NE(0, lamda(str));
    IGNORE_UNUSED_VAR(str0, str1, str2);
}

TEST(coding_style, west_const)
{
    // @@@ sample begin 0:1

    char              str[] = "hehe";  // 配列strに書き込み可能
    const char*       str0  = str;  // str0が指すオブジェクトはconstなので、*str0への書き込み不可
    char* const       str1  = str;  // str1がconstなので、str1への代入不可
    const char* const str2  = str;  // *str2への書き込み不可、str2への代入不可

    auto lamda = [](const char(&str_ref)[5]) {  // str_refは配列へのconstリファレンス
        int ret = 0;

        for (const char& a : str_ref) {  // aはchar constリファレンス
            ret += a;
        }
        return ret;
    };
    // @@@ sample end

    ASSERT_NE(0, lamda(str));
    IGNORE_UNUSED_VAR(str0, str1, str2);
}
}  // namespace
