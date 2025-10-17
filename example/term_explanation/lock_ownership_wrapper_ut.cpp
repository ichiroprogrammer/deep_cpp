#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace no_lock_guard {
// @@@ sample begin 0:0

struct Conflict {
    void increment()
    // @@@ sample end
    // @@@ sample begin 0:1
    {
        mtx_.lock();  // ++count_の排他のためのロック

        ++count_;

        mtx_.unlock();  // 上記のアンロック
    }
    // @@@ sample end
    // @@@ sample begin 0:2

    uint32_t   count_ = 0;
    std::mutex mtx_{};
};
// @@@ sample end

TEST(ExpTerm, thread)
{
    // @@@ sample begin 0:3

    Conflict c;

    constexpr uint32_t inc_per_thread = 5'000'000;
    constexpr uint32_t expected       = 2 * inc_per_thread;

    auto worker = [&c] {
        for (uint32_t i = 0; i < inc_per_thread; ++i) {
            c.increment();
        }
    };

    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();  // スレッドの終了待ち
    t2.join();  // スレッドの終了待ち
                // 注意: join()もdetach()も呼ばずにスレッドオブジェクトが
                // デストラクトされると、std::terminateが呼ばれる

    ASSERT_EQ(c.count_, expected);
    // @@@ sample end
}
}  // namespace no_lock_guard
namespace lock_guard {
// @@@ sample begin 1:0

struct Conflict {
    void increment()
    // @@@ sample end
    // @@@ sample begin 1:1
    {
        std::lock_guard<std::mutex> lock{mtx_};  // lockオブジェクトのコンストラクタでmtx_.lock()が呼ばれる
                                                 // ++count_の排他
        ++count_;

    }  // lockオブジェクトのデストラクタでmtx_.unlock()が呼ばれる
    // @@@ sample end
    // @@@ sample begin 1:2
    uint32_t   count_ = 0;
    std::mutex mtx_{};
};
// @@@ sample end

TEST(ExpTerm, thread)
{
    // @@@ sample begin 1:3

    Conflict c;

    constexpr uint32_t inc_per_thread = 5'000'000;
    constexpr uint32_t expected       = 2 * inc_per_thread;

    auto worker = [&c] {
        for (uint32_t i = 0; i < inc_per_thread; ++i) {
            c.increment();
        }
    };

    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();  // スレッドの終了待ち
    t2.join();  // スレッドの終了待ち
                // 注意: join()もdetach()も呼ばずにスレッドオブジェクトが
                // デストラクトされると、std::terminateが呼ばれる

    ASSERT_EQ(c.count_, expected);
    // @@@ sample end
}
}  // namespace lock_guard

namespace {

// @@@ sample begin 2:0

class IntQueue {
public:
    void push(int v)
    {
        {
            std::lock_guard<std::mutex> lg{mtx_};  // ロック取得
            q_.push(v);
        }  // ロック解放

        cv_.notify_one();  // 待機中のスレッドを1つ起床
                           // 注: ロック解放後に呼び出すことで、起床したスレッドがすぐにロックを取得できる
    }
    // @@@ sample end
    // @@@ sample begin 2:1

    int pop_ng()
    {
        std::unique_lock<std::mutex> lock{mtx_};
        cv_.wait(lock);  // NG: Spurious Wakeup対策なし
                         // 起床時に条件を再確認しないため、
                         // q_.empty() が true のまま起床する可能性がある
        int v = q_.front();
        q_.pop();  // 条件未確認アクセス（危険）

        return v;
    }
    // @@@ sample end
    // @@@ sample begin 2:2

    int pop_ok()
    {
        std::unique_lock<std::mutex> lock{mtx_};
        cv_.wait(lock, [&q_ = q_] { return !q_.empty(); });  // waitの述語が true になるまで待機(Spurious Wakeup対策)
        // wait()の動作:
        // 1. 述語を評価してtrueならすぐreturn
        // 2. falseなら: unlock() → 通知待機 → 通知受信 → lock() → 述語再評価
        // 3. 述語がtrueになるまで2を繰り返す

        int v = q_.front();
        q_.pop();  // ここでは、q_.empty()は必ずfalse
        return v;
    }
    // @@@ sample end

    // @@@ sample begin 2:3
private:
    std::mutex              mtx_{};
    std::condition_variable cv_{};
    std::queue<int>         q_{};
};
// @@@ sample end

TEST(ExpTerm, thread)
{
    // @@@ sample begin 2:4

    IntQueue           iq;
    constexpr int      end_data       = -1;
    constexpr uint32_t push_count_max = 10;

    // Producer
    std::thread t1([&iq] {
        for (uint32_t i = 0; i < push_count_max; ++i) {
            iq.push(100 + i);
        }

        iq.push(end_data);  // t2が-1を受信したらt2のループ終了
    });

    uint32_t pop_count = 0;

    std::thread t2([&iq, &pop_count] {
        for (;;) {
            if (int v = iq.pop_ok(); v == -1) {
                break;
            }
            else {
                ++pop_count;
            }
        }
    });

    t1.join();  // スレッドの終了待ち
    t2.join();  // スレッドの終了待ち

    ASSERT_EQ(push_count_max, pop_count);
    // @@@ sample end
}
}  // namespace
