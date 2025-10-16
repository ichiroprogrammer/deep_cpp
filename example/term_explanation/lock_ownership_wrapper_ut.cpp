#include <atomic>
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
