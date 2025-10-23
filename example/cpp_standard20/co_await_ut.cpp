#include <coroutine>
#include <iostream>
#include <memory>
#include <sstream>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace use_coroutine {

// @@@ sample begin 0:0

class Task {  // コルーチンが返す型
public:
    /// @struct promise_type
    /// @brief コルーチンのライフサイクルを管理する構造体
    struct promise_type {
        /// @brief コルーチンから Task 型のオブジェクトを返す関数
        /// @return Taskオブジェクト
        Task get_return_object() { return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }

        /// @brief コルーチンの最初のサスペンドポイント
        /// @return 常にサスペンドするオブジェクトを返す
        std::suspend_always initial_suspend() { return {}; }

        /// @brief コルーチンの最後のサスペンドポイント
        /// @return 常にサスペンドするオブジェクトを返す
        std::suspend_always final_suspend() noexcept { return {}; }

        /// @brief コルーチン内で例外が発生した場合に呼び出される
        /// @details コルーチン内で未処理の例外が発生した場合に、プロセスを終了する
        void unhandled_exception() { std::exit(1); }

        /// @brief コルーチンが終了した際に呼び出される
        /// @details co_return で値が返されない場合に呼び出されるが、何も行わない
        void return_void() {}
    };

    /// @brief Task のコンストラクタ
    /// @param h コルーチンハンドル
    Task(std::coroutine_handle<promise_type> h) : coro{h} {}

    /// @brief コルーチンの呼び出し回数に基づいた文字列を返す
    /// @return 呼び出し回数に応じた "call X" という文字列
    std::string get_value() { return "call " + std::to_string(called); }

    /// @brief コルーチンを再開する
    /// @details コルーチンが終了していなければ再開し、呼び出し回数をカウントする
    /// @return コルーチンが完了していなければ true、完了していれば false
    bool resume()
    {
        ++called;                 // コルーチンを呼び出した回数をカウント
        if (!coro.done()) {       // コルーチンが完了していなければ
            coro.resume();        // 再開
            return !coro.done();  // 再開後も完了していなければ true を返す
        }
        return false;  // すでに完了している場合は false を返す
    }

    /// @brief Task のデストラクタ
    /// @details コルーチンハンドルが有効であれば破棄する
    ~Task()
    {
        if (coro) coro.destroy();
    }

private:
    std::coroutine_handle<promise_type> coro;        ///< コルーチンハンドル
    uint32_t                            called = 0;  ///< コルーチンが再開された回数
};

/// @brief コルーチンを生成する関数
/// @return Taskオブジェクト
Task gen_coroutine()
{
    co_await std::suspend_always{};  // 最初のサスペンドポイント
    co_await std::suspend_always{};  // 2回目のサスペンドポイント
    co_return;                       // コルーチン終了
}
// @@@ sample end

TEST(ExpTerm, use_co_await)
{
    // @@@ sample begin 0:1

    Task    task  = gen_coroutine();  // gen_coroutine から Task オブジェクトを生成
    int32_t calls = 0;

    /// @test コルーチンを resume() で再開し、完了するまでループする
    while (task.resume()) {  // コルーチンが完了していない間、再開
        switch (calls) {
        case 0:
            /// @test コルーチンが1回目の再開後、"call 1" が返されることを確認
            ASSERT_EQ("call 1", task.get_value());
            break;
        case 1:
            /// @test コルーチンが2回目の再開後、"call 2" が返されることを確認
            ASSERT_EQ("call 2", task.get_value());
            break;
        case 2:
            /// @test コルーチンが3回目の再開後、"call 3" が返されることを確認
            ASSERT_EQ("call 3", task.get_value());
            break;
        }
        ++calls;
    }

    /// @test コルーチンが 2 回 resume された後に終了していることを確認
    ASSERT_EQ(2, calls);
    // @@@ sample end
}
}  // namespace use_coroutine
namespace no_use_coroutine {
// @@@ sample begin 1:0

/// @enum CoroutineState
/// @brief ManualCoroutine の状態を表す enum 型
enum class CoroutineState {
    Start,             ///< コルーチンが開始された状態
    FirstSuspension,   ///< コルーチンが最初にサスペンドされた状態
    SecondSuspension,  ///< コルーチンが2回目にサスペンドされた状態
    Finished           ///< コルーチンが完了した状態
};

/// @brief コルーチンの状態を保持し、進行を管理するためのクラス
class ManualCoroutine {
public:
    /// @brief コルーチンの代わりに状態を進行させる関数
    /// @details コルーチンの状態に基づいて進行し、コルーチンのように振る舞う
    /// @return コルーチンが継続可能なら true、終了していれば false を返す
    bool resume()
    {
        ++called;  // コルーチンの再開回数をカウント
        switch (state) {
        case CoroutineState::Start:
            state = CoroutineState::FirstSuspension;
            return true;  // 継続可能

        case CoroutineState::FirstSuspension:
            state = CoroutineState::SecondSuspension;
            return true;  // 継続可能

        case CoroutineState::SecondSuspension:
            state = CoroutineState::Finished;
            return false;  // 終了

        case CoroutineState::Finished:
            return false;  // 既に終了している
        }
        assert(false);  // 不正な状態（理論的には到達しないはず）
        return false;
    }

    /// @brief 呼び出し回数に基づいた文字列を返す
    /// @return "call X" という形式の文字列（X は呼び出し回数）
    std::string get_value() { return "call " + std::to_string(called); }

private:
    uint32_t       called = 0;                      ///< コルーチンが再開された回数
    CoroutineState state  = CoroutineState::Start;  ///< 現在のコルーチンの状態
};
// @@@ sample end

TEST(ExpTerm, no_co_await)
{
    // @@@ sample begin 1:1

    auto    manual_coroutine = ManualCoroutine{};
    int32_t calls            = 0;

    while (manual_coroutine.resume()) {  // コルーチンを再開する
        switch (calls) {
        case 0:
            /// @test 1回目の再開後に "call 1" が返されることを確認
            ASSERT_EQ("call 1", manual_coroutine.get_value());
            break;
        case 1:
            /// @test 2回目の再開後に "call 2" が返されることを確認
            ASSERT_EQ("call 2", manual_coroutine.get_value());
            break;
        case 2:
            /// @test 3回目の再開後に "call 3" が返されることを確認
            ASSERT_EQ("call 3", manual_coroutine.get_value());
            break;
        }
        ++calls;  ///< コルーチンの再開回数をインクリメント
    }

    /// @test コルーチンが2回 resume された後に終了していることを確認
    ASSERT_EQ(2, calls);
    // @@@ sample end
}
}  // namespace no_use_coroutine
