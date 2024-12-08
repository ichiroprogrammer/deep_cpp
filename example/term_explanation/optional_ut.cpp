#include <iostream>
#include <optional>
#include <string>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

/// @brief 指定されたファイル名から拡張子を取得する。
/// @param filename ファイル名（パスを含む場合も可）
/// @return 拡張子を文字列として返す。拡張子がない場合は std::nullopt を返す。
std::optional<std::string> file_extension(std::string const& filename)
{
    size_t pos = filename.rfind('.');
    if (pos == std::string::npos || pos == filename.length() - 1) {
        return std::nullopt;  // 値が存在しない
    }
    return filename.substr(pos + 1);
}
// @@@ sample end

TEST(ExpTerm, optional_0)
{
    // @@@ sample begin 0:1

    auto ret0 = file_extension("xxx.yyy");

    ASSERT_TRUE(ret0);  // 値を保持している
    ASSERT_EQ("yyy", *ret0);

    auto ret1 = file_extension("xxx");

    ASSERT_FALSE(ret1);  // 値を保持していない
    // ASSERT_THROW(*ret1, std::exception);  // 未定義動作(エクセプションは発生しない)
    ASSERT_THROW(ret1.value(), std::bad_optional_access);  // 値非保持の場合、エクセプション発生
    // @@@ sample end
}

// @@@ sample begin 1:0

class HeavyResource {
public:
    HeavyResource() : large_erea_{0xdeadbeaf}
    {  // large_erea_[0]を44にする
        initialied = true;
    }
    bool     is_ready() const noexcept { return large_erea_[0] == 0xdeadbeaf; }
    uint32_t operator[](size_t index) const noexcept { return large_erea_[index]; }

    static bool initialied;

private:
    uint32_t large_erea_[1024];
};
bool HeavyResource::initialied;
// @@@ sample end

TEST(ExpTerm, optional_1)
{
    // @@@ sample begin 1:1

    std::optional<HeavyResource> resource;

    // resourceの内部のHeavyResourceは未初期化
    ASSERT_FALSE(resource.has_value());
    ASSERT_FALSE(HeavyResource::initialied);
    ASSERT_NE(0xdeadbeaf, (*resource)[0]);  // 未定義動作

    // resourceの内部のHeavyResourceの遅延初期化
    resource.emplace();  // std::optionalの内部でplacement newが実行される

    // ここから下は定義動作
    ASSERT_TRUE(HeavyResource::initialied);  // resourceの内部のHeavyResourceは初期化済み
    ASSERT_TRUE(resource.has_value());

    ASSERT_TRUE(resource->is_ready());
    ASSERT_EQ(0xdeadbeaf, (*resource)[0]);
    //  @@@ sample end
}
}  // namespace
