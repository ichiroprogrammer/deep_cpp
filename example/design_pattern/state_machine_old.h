#pragma once
#include <string_view>

// @@@ sample begin 0:0

extern std::string_view ThreadOldStyleStateStr() noexcept;  // 現在の状態を文字列で返す、単体テストやロギング用の関数

// スレッドの状態を変えるためのコマンドを発行するための関数群
extern void ThreadOldStyleRun();
extern void ThreadOldStyleAbort();
extern void ThreadOldStyleSuspend();
// @@@ sample end
