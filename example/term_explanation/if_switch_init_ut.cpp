#include "gtest_wrapper.h"

#include "nstd_type2str.h"
#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

class OperationResult {
public:
    enum class ErrorCode { NoError, ErrorPattern1, ErrorPattern2, ErrorPattern3 };
    bool      IsError() const noexcept;
    ErrorCode Get() const noexcept;
              operator bool() const noexcept { return IsError(); }

private:
    // 何らかの定義
};

OperationResult DoOperation();                                 // 何らかの処理
void            RecoverOperation(OperationResult::ErrorCode);  // リカバリ処理
// @@@ sample end

bool                       OperationResult::IsError() const noexcept { return false; }
OperationResult::ErrorCode OperationResult::Get() const noexcept
{
    return OperationResult::ErrorCode::ErrorPattern1;
}

void            RecoverOperation(OperationResult::ErrorCode) {}
OperationResult DoOperation() { return OperationResult{}; }

TEST(TermExp, if_switch_init0)
{
    // @@@ sample begin 1:0

    for (auto result = DoOperation(); result.IsError(); result = DoOperation()) {
        RecoverOperation(result.Get());  // エラー処理
    }

    // 以下、成功時の処理
    // @@@ sample end
}

TEST(TermExp, while)
{
    // @@@ sample begin 2:0

    while (auto result = DoOperation()) {  // resultはboolへの暗黙の型変換が行われる
        // エラー処理
    }
    // resultはスコープアウトする
    // @@@ sample end
}

TEST(TermExp, if_switch_init1)
{
    // @@@ sample begin 3:0

    if (auto result = DoOperation(); !result.IsError()) {
        // 成功処理
    }
    else {
        RecoverOperation(result.Get());  // エラー処理
    }
    // resultはスコープアウトする
    // @@@ sample end
}

#if __cplusplus >= 202002L  // C++20
// @@@ sample begin 4:0

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
#endif

TEST(TermExp, if_switch_init2)
{
    // @@@ sample begin 5:0

    switch (auto result = DoOperation(); result.Get()) {
    case OperationResult::ErrorCode::ErrorPattern1:
        RecoverOperation(result.Get());  // エラー処理
        break;
        // エラー処理のいくつかのパターン
    case OperationResult::ErrorCode::NoError:
        // 成功処理
    default:
        assert(false);  // ここには来ないはず
    }
    // resultはスコープアウトする
    // @@@ sample end
}

}  // namespace
