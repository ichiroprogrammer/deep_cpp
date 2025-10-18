#include <atomic>
#include <thread>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace conflict {
// @@@ sample begin 0:0

struct Conflict {
    void     increment() { ++count_; }  // 非アトミック（データレースの原因）
    uint32_t count_ = 0;
};

void worker(Conflict& c, int n)
{
    for (int i = 0; i < n; ++i) {
        c.increment();
    }
}
// @@@ sample end

TEST(ExpTerm, thread)
{
    // @@@ sample begin 0:1

    Conflict c;

    constexpr uint32_t inc_per_thread = 5'000'000;
    constexpr uint32_t expected       = 2 * inc_per_thread;

    std::thread t1(worker, std::ref(c), inc_per_thread);  // worker関数を使用したスレッドの起動
                                                          // workerにcのリファレンス渡すため、std::refを使用

    std::thread t2([&c] {  // ラムダを使用したによるスレッドの起動
        for (uint32_t i = 0; i < inc_per_thread; ++i) {
            c.increment();
        }
    });

    t1.join();  // スレッドの終了待ち
    t2.join();  // スレッドの終了待ち
                // 注意: join()もdetach()も呼ばずにスレッドオブジェクトが
                // デストラクトされると、std::terminateが呼ばれる

    // ASSERT_EQ(c.count_, expected);  t1とt2が++count_が競合するためこのテストは成立しないため、
    //                                 一例では次のようになる  c.count_: 6825610 expected: 10000000
    ASSERT_NE(c.count_, expected);
    // @@@ sample end
}
}  // namespace conflict

namespace no_conflict {
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

void worker(Conflict& c, int n)
{
    for (int i = 0; i < n; ++i) {
        c.increment();
    }
}
// @@@ sample end

TEST(ExpTerm, thread)
{
    // @@@ sample begin 1:3

    Conflict c;

    constexpr uint32_t inc_per_thread = 5'000'000;
    constexpr uint32_t expected       = 2 * inc_per_thread;

    std::thread t1(worker, std::ref(c), inc_per_thread);  // worker関数を使用したスレッドの起動
                                                          // workerにcのリファレンス渡すため、std::refを使用

    std::thread t2([&c] {  // ラムダを使用したによるスレッドの起動
        for (uint32_t i = 0; i < inc_per_thread; ++i) {
            c.increment();
        }
    });

    t1.join();  // スレッドの終了待ち
    t2.join();  // スレッドの終了待ち
                // 注意: join()もdetach()も呼ばずにスレッドオブジェクトが
                // デストラクトされると、std::terminateが呼ばれる

    ASSERT_EQ(c.count_, expected);
    // @@@ sample end
}
}  // namespace no_conflict
namespace atomic {
// @@@ sample begin 2:0

struct Conflict {
    void increment()
    {
        ++count_;  // ++count_は「count_の値の呼び出し -> その値のインクリメント、その値のcount_への書き戻し」である
                   // この一連の操作は排他的(アトミック)に行われる

    }  // lockオブジェクトのデストラクタでmtx_.unlock()が呼ばれる
    std::atomic<uint32_t> count_ = 0;
};

void worker(Conflict& c, int n)
{
    for (int i = 0; i < n; ++i) {
        c.increment();
    }
}
// @@@ sample end

TEST(ExpTerm, thread)
{
    // @@@ sample begin 2:1

    Conflict c;

    constexpr uint32_t inc_per_thread = 5'000'000;
    constexpr uint32_t expected       = 2 * inc_per_thread;

    std::thread t1(worker, std::ref(c), inc_per_thread);  // worker関数を使用したスレッドの起動
                                                          // workerにcのリファレンス渡すため、std::refを使用

    std::thread t2([&c] {  // ラムダを使用したスレッドの起動
        for (uint32_t i = 0; i < inc_per_thread; ++i) {
            c.increment();
        }
    });

    t1.join();  // スレッドの終了待ち
    t2.join();  // スレッドの終了待ち
                // 注意: join()もdetach()も呼ばずにスレッドオブジェクトが
                // デストラクトされると、std::terminateが呼ばれる

    ASSERT_EQ(c.count_, expected);
    // @@@ sample end
}
}  // namespace atomic
