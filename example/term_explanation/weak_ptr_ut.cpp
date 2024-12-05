#include <cstdint>
#include <memory>
#include <type_traits>

#include "gtest_wrapper.h"

namespace not_leak {
// @@@ sample begin 0:0

class Y;
class X final {
public:
    explicit X() noexcept { ++constructed_counter; }
    ~X() { --constructed_counter; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<Y> y_{};
};
uint32_t X::constructed_counter;

class Y final {
public:
    explicit Y() noexcept { ++constructed_counter; }
    ~Y() { --constructed_counter; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<X> x_{};
};
uint32_t Y::constructed_counter;
// @@@ sample end

TEST(WeakPtr, not_leak)
{
#ifndef __clang_analyzer__
    // @@@ sample begin 1:0

    {  // ステップ1
        ASSERT_EQ(X::constructed_counter, 0);
        ASSERT_EQ(Y::constructed_counter, 0);

        auto x0 = std::make_shared<X>();
        auto y0 = std::make_shared<Y>();

        ASSERT_EQ(x0.use_count(), 1);

        ASSERT_EQ(y0.use_count(), 1);

        ASSERT_EQ(X::constructed_counter, 1);
        ASSERT_EQ(Y::constructed_counter, 1);
        // @@@ sample end
        // @@@ sample begin 1:1
        // ステップ2

        auto x1 = x0;
        auto y1 = y0;

        ASSERT_EQ(X::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない
        ASSERT_EQ(Y::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない

        ASSERT_EQ(x1.use_count(), 2);  // コピーしたため、参照カウントが増えた
        ASSERT_EQ(y1.use_count(), 2);  // コピーしたため、参照カウントが増えた
        // @@@ sample end
        // @@@ sample begin 1:2
        // ステップ3

        auto x2 = std::move(x1);
        auto y2 = std::move(y1);

        ASSERT_EQ(x1.use_count(), 0);  // ムーブしたため、参照カウントが0に
        ASSERT_EQ(y1.use_count(), 0);  // ムーブしたため、参照カウントが0に

        ASSERT_EQ(x0.use_count(), 2);  // x0からムーブしていないので参照カウントは不変
        ASSERT_EQ(y0.use_count(), 2);  // x0からムーブしていないので参照カウントは不変
        // @@@ sample end
        // @@@ sample begin 1:3

    }  // この次の行で、x0、x2、y0、y2はスコープアウトし、X、Yオブジェクトは解放される

    ASSERT_EQ(X::constructed_counter, 0);  // Xオブジェクトの解放の確認
    ASSERT_EQ(Y::constructed_counter, 0);  // Yオブジェクトの解放の確認
    // @@@ sample end
#endif
}
}  // namespace not_leak
namespace leak {

// @@@ sample begin 2:0

class Y;
class X final {
public:
    explicit X() noexcept { ++constructed_counter; }
    ~X() { --constructed_counter; }

    void Register(std::shared_ptr<Y> y) { y_ = y; }

    std::shared_ptr<Y> const& ref_y() const noexcept { return y_; }

    bool DoSomething() { return true; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<Y> y_{};
};
uint32_t X::constructed_counter;

class Y final {
public:
    explicit Y() noexcept { ++constructed_counter; }
    ~Y() { --constructed_counter; }

    void Register(std::shared_ptr<X> x) { x_ = x; }

    std::shared_ptr<X> const& ref_x() const noexcept { return x_; }

    bool DoSomething() { return x_ ? x_->DoSomething() : false; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<X> x_{};
};
uint32_t Y::constructed_counter;
// @@@ sample end
#ifndef SANITIZER

TEST(WeakPtr, leak)
{
    // @@@ sample begin 2:1

    {
        ASSERT_EQ(X::constructed_counter, 0);
        ASSERT_EQ(Y::constructed_counter, 0);

        auto x0 = std::make_shared<X>();

        ASSERT_EQ(x0.use_count(), 1);
        ASSERT_EQ(x0->ref_y().use_count(), 0);  // X::y_は何も管理していない

        ASSERT_EQ(X::constructed_counter, 1);  // Xオブジェクトは1つ生成された
        // @@@ sample end
        // @@@ sample begin 2:2

        {
            auto y0 = std::make_shared<Y>();

            ASSERT_EQ(Y::constructed_counter, 1);  // Yオブジェクトは1つ生成された
            ASSERT_EQ(y0.use_count(), 1);
            ASSERT_EQ(y0->ref_x().use_count(), 0);  // Y::x_は何も管理していない

            ASSERT_FALSE(y0->DoSomething());  // Y::DoSomethingの処理をX::DoSomethingに委譲

            x0->Register(y0);  // これによりx0とy0が互いに所有し合う(循環参照)
            y0->Register(x0);  // これによりx0とy0が互いに所有し合う(循環参照)
            ASSERT_EQ(X::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない
            ASSERT_EQ(Y::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない

            ASSERT_TRUE(y0->DoSomething());  // Y::DoSomethingの処理をX::DoSomethingに委譲
            // @@@ sample end
            // @@@ sample begin 2:3

            ASSERT_EQ(x0.use_count(), 2);  // x0、y0が相互に参照するので参照カウントが2に
            ASSERT_EQ(y0->ref_x().use_count(), 2);
            ASSERT_EQ(y0.use_count(), 2);  // x0、y0が相互に参照するので参照カウントが2に
            ASSERT_EQ(x0->ref_y().use_count(), 2);
        }  //ここでy0がスコープアウトする

        ASSERT_EQ(x0->ref_y().use_count(), 1);  // y0がスコープアウトしたため、
                                                // Yオブジェクトの参照カウントが減った
        // @@@ sample end
        // @@@ sample begin 2:4
    }  // この次の行で、x0はスコープアウトする

    ASSERT_EQ(X::constructed_counter, 1);  // Xオブジェクトは未開放であり、リークが発生
    ASSERT_EQ(Y::constructed_counter, 1);  // Yオブジェクトは未開放であり、リークが発生
    // @@@ sample end
}
#endif
}  // namespace leak

namespace not_leak_by_weak {

// @@@ sample begin 3:0

class Y;
class X final {
public:
    explicit X() noexcept { ++constructed_counter; }
    ~X() { --constructed_counter; }

    void Register(std::shared_ptr<Y> y) { y_ = y; }

    std::shared_ptr<Y> const& ref_y() const noexcept { return y_; }

    bool DoSomething() { return true; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<Y> y_{};
};
uint32_t X::constructed_counter;

class Y final {
public:
    explicit Y() noexcept { ++constructed_counter; }
    ~Y() { --constructed_counter; }

    void Register(std::shared_ptr<X> x) { x_ = x; }

    std::weak_ptr<X> const& ref_x() const noexcept { return x_; }
    // @@@ sample end
    // @@@ sample begin 3:1

    bool DoSomething()
    {
        if (auto x = x_.lock(); x) {  // weak_ptrからshared_ptrの生成
            static_assert(std::is_same_v<std::shared_ptr<X>, decltype(x)>);
            return x->DoSomething();
        }
        else {
            return false;
        }
    }
    // @@@ sample end
    // @@@ sample begin 3:2

    static uint32_t constructed_counter;

private:
    std::weak_ptr<X> x_{};
};
uint32_t Y::constructed_counter;
// @@@ sample end
#ifndef SANITIZER

TEST(WeakPtr, not_leak_by_weak)
{
    // @@@ sample begin 3:3

    {
        ASSERT_EQ(X::constructed_counter, 0);
        ASSERT_EQ(Y::constructed_counter, 0);

        auto x0 = std::make_shared<X>();
        auto y0 = std::make_shared<Y>();

        ASSERT_EQ(x0.use_count(), 1);
        ASSERT_EQ(x0->ref_y().use_count(), 0);  // X::y_は何も管理していない

        ASSERT_EQ(X::constructed_counter, 1);  // Xオブジェクトは1つ生成された
        ASSERT_EQ(Y::constructed_counter, 1);  // Yオブジェクトは1つ生成された

        ASSERT_EQ(y0.use_count(), 1);
        ASSERT_EQ(y0->ref_x().use_count(), 0);  // Y::x_は何も管理していない

        ASSERT_FALSE(y0->DoSomething());  // Y::DoSomethingの処理をX::DoSomethingに委譲

        x0->Register(y0);  // これによりy0にy0を渡す。y0 -> x0の参照はshared_ptrによって行う
        y0->Register(x0);  // これによりx0にy0を渡すが、y0 -> x0の参照はweak_ptrによって行う
        ASSERT_EQ(X::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない
        ASSERT_EQ(Y::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない

        ASSERT_TRUE(y0->DoSomething());  // Y::DoSomethingの処理をX::DoSomethingに委譲

        ASSERT_EQ(x0.use_count(), 1);  // Xオブジェクトはx0に所有されるが、y0には所有されない
        ASSERT_EQ(y0->ref_x().use_count(), 1);  // weak_ptr<X>::use_count
        ASSERT_EQ(y0.use_count(), 2);  // Yオブジェクトはy0とx0から共有所有されるため
        ASSERT_EQ(x0->ref_y().use_count(), 2);  // Yオブジェクトはy0とx0から共有所有されるため
    }  // この次の行で、x0、y0はスコープアウトする。

    ASSERT_EQ(X::constructed_counter, 0);  // Xオブジェクトは開放済み
    ASSERT_EQ(Y::constructed_counter, 0);  // Yオブジェクトは開放済み
    // @@@ sample end
}
#endif
}  // namespace not_leak_by_weak
