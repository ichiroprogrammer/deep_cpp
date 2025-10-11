#include <cstdint>
#include <memory>
#include <type_traits>

#include "gtest_wrapper.h"

namespace not_leak_by_weak {

// @@@ sample begin 0:0

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
    // @@@ sample begin 0:1

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
    // @@@ sample begin 0:2

    static uint32_t constructed_counter;

private:
    std::weak_ptr<X> x_{};
};
uint32_t Y::constructed_counter;
// @@@ sample end
#ifndef SANITIZER

TEST(WeakPtr, not_leak_by_weak)
{
    // @@@ sample begin 0:3

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
        ASSERT_EQ(y0.use_count(), 2);           // Yオブジェクトはy0とx0から共有所有されるため
        ASSERT_EQ(x0->ref_y().use_count(), 2);  // Yオブジェクトはy0とx0から共有所有されるため
    }                                           // この次の行で、x0、y0はスコープアウトする。

    ASSERT_EQ(X::constructed_counter, 0);  // Xオブジェクトは開放済み
    ASSERT_EQ(Y::constructed_counter, 0);  // Yオブジェクトは開放済み
    // @@@ sample end
}
#endif
}  // namespace not_leak_by_weak
