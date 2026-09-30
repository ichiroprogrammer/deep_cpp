#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "gtest_wrapper.h"

// @@@ sample begin 0:0

class Servant {  // 実際の処理。ワーカスレッドからのみ呼ばれるため、ロックは不要
public:
    int Add(int a, int b) { return a + b; }
};

class ActiveObject {
public:
    // 全メンバの初期化後にworker_が構築され、その時点でワーカスレッドがdispatch()を開始する
    ActiveObject() : worker_{&ActiveObject::dispatch, this} {}
    ~ActiveObject();

    ActiveObject(ActiveObject const&)            = delete;  // worker_がthisを保持するため、
    ActiveObject& operator=(ActiveObject const&) = delete;  // コピーもムーブも禁止する

    // Proxyに相当する。Servant::Addの呼び出しを要求としてキューに積み、結果を待たずに戻る。
    // 引数は値でキャプチャする。要求の実行時には、呼び出し側の変数はすでに存在しない可能性があるためである。
    // thisのキャプチャが安全なのは、デストラクタがワーカスレッドの終了を待つためである。
    std::future<int> Add(int a, int b)
    {
        return enqueue([this, a, b] { return servant_.Add(a, b); });
    }

private:
    template <typename F>
    auto enqueue(F func) -> std::future<decltype(func())>;
    void dispatch();  // Schedulerに相当する。ワーカスレッドのメイン関数

    Servant servant_{};  // ワーカスレッドからのみアクセスされる

    // 以下はクライアントスレッドとワーカスレッドで共有される。requests_とstop_はmtx_で保護する
    std::list<std::packaged_task<void()>> requests_{};   // Activation Listに相当する。未実行の要求
    bool                                  stop_{false};  // デストラクタからの終了要求
    std::mutex                            mtx_{};
    std::condition_variable cv_{};  // 要求の追加と終了要求をワーカスレッドに通知する

    // ワーカスレッドは構築と同時に他のメンバへアクセスし始める。
    // メンバは宣言順に初期化されるため、worker_は必ず最後に宣言する。
    std::thread worker_;
};

// func()をワーカスレッドで実行する要求を生成してキューに積み、結果を受け取るfutureを返す。
//
// std::packaged_task<R()>は、func()の戻り値、またはfunc()が送出した例外をfutureに格納する。
// 戻り値型Rはメンバ関数ごとに異なるため、そのままでは1つのキューに積めない。
// そこで、R()のtaskを呼び出すだけのラムダ式を作り、packaged_task<void()>に包んでキューに積む。
// 外側のpackaged_task<void()>は、ムーブ専用のラムダ式を保持する入れ物として使うだけであり、
// そのfuture(future<void>)は使用しない。
// なお、std::function<void()>はコピー構築可能な関数オブジェクトしか保持できないため、
// ムーブ専用のtaskをキャプチャしたラムダ式を保持できない。
template <typename F>
auto ActiveObject::enqueue(F func) -> std::future<decltype(func())>
{
    using R = decltype(func());

    auto task   = std::packaged_task<R()>{std::move(func)};
    auto future = task.get_future();  // taskをムーブする前に取得する
    {
        std::lock_guard<std::mutex> lock{mtx_};
        requests_.emplace_back([task = std::move(task)]() mutable { task(); });
    }
    cv_.notify_one();  // ロック解放後に通知し、起床したワーカスレッドがすぐにロックを取れるようにする

    return future;
}

inline void ActiveObject::dispatch()
{
    for (;;) {
        std::packaged_task<void()> request;
        {
            std::unique_lock<std::mutex> lock{mtx_};

            // 要求が来るか終了要求が出るまでウエイトする。
            cv_.wait(lock, [this] { return !requests_.empty() || stop_; });
            if (requests_.empty()) {  // 終了要求済みかつキューが空。残りの要求は実行済み
                return;
            }
            request = std::move(requests_.front());
            requests_.pop_front();
        }
        request();  // 内側のtask()が実行され、結果がfutureに届く。ロックの外で実行する。実行中もクライアントスレッドは要求を追加できる
    }
}

inline ActiveObject::~ActiveObject()
{
    {
        std::lock_guard<std::mutex> lock{mtx_};
        stop_ = true;   // dispatchのwaitのブロックから起こす
    }
    cv_.notify_one();
    worker_.join();  // 受け付け済みの要求をすべて実行し終えるまで待つ。この後にservant_等が破棄される
}
// @@@ sample end

TEST(ActiveObject, Add)
{
    // @@@ sample begin 1:0

    ActiveObject ao{};

    std::future<int> future = ao.Add(1, 2);  // 直ちに戻る
    // この間、呼び出し側は他の処理を行える

    ASSERT_EQ(future.get(), 3);  // 結果が出るまでブロックする
    // @@@ sample end
}
