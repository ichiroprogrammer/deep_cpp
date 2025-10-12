#include <cstdint>
#include <memory>
#include <type_traits>

#include "gtest_wrapper.h"

namespace normal_shared {
// @@@ sample begin 0:0

class Y;
class X final {
public:
    explicit X() noexcept { ++constructed_counter; }
    ~X() { --constructed_counter; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<Y> y_{};  // 初期化状態では、y_はオブジェクトの所有しない(use_count()==0)
};
uint32_t X::constructed_counter;

class Y final {
public:
    explicit Y() noexcept { ++constructed_counter; }
    ~Y() { --constructed_counter; }

    static uint32_t constructed_counter;

private:
    std::shared_ptr<X> x_{};  // 初期化状態では、x_はオブジェクトの所有しない(use_count()==0)
};
uint32_t Y::constructed_counter;
// @@@ sample end

TEST(WeakPtr, normal_shared)
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

        auto x1 = x0;  // x0が保有するオブジェクトをx1にも所有させる
        auto y1 = y0;  // y0が保有するオブジェクトをy1にも所有させる

        ASSERT_EQ(X::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない
        ASSERT_EQ(Y::constructed_counter, 1);  // 新しいオブジェクトが生成されるわけではない

        ASSERT_EQ(x1.use_count(), 2);  // コピーしたため、参照カウントが増えた
        ASSERT_EQ(y1.use_count(), 2);  // コピーしたため、参照カウントが増えた
        // @@@ sample end
        // @@@ sample begin 1:2
        // ステップ3

        auto x2 = std::move(x1);  // x1が保持する所有権をx2に移動(この後x1はオブジェクトを所有しない)
        auto y2 = std::move(y1);  // y1が保持する所有権をy2に移動(この後y1はオブジェクトを所有しない)

        ASSERT_EQ(x1.use_count(), 0);  // ムーブしたため、参照カウントが0に
        ASSERT_EQ(y1.use_count(), 0);  // ムーブしたため、参照カウントが0に

        ASSERT_EQ(x0.use_count(), 2);  // x0からムーブしていないので参照カウントは不変
        ASSERT_EQ(y0.use_count(), 2);  // y0からムーブしていないので参照カウントは不変
        // @@@ sample end
        // @@@ sample begin 1:3

    }  // この次の行で、x0、x2、y0、y2はスコープアウトし、X、Yオブジェクトは解放される

    ASSERT_EQ(X::constructed_counter, 0);  // Xオブジェクトの解放の確認
    ASSERT_EQ(Y::constructed_counter, 0);  // Yオブジェクトの解放の確認
    // @@@ sample end
#endif
}
}  // namespace normal_shared
namespace leak {

// @@@ sample begin 2:0

class Y;
class X final {
public:
    explicit X() noexcept { ++constructed_counter; }
    ~X() { --constructed_counter; }

    void Register(std::shared_ptr<Y> y) { y_ = y; }

    std::shared_ptr<Y> const& ref_y() const noexcept { return y_; }

    // 自身の状態を返す ("X alone" または "X with Y")
    std::string WhoYouAre() const;

    // y_が保持するオブジェクトの状態を返す ("None" またはY::WhoYouAre()に委譲)
    std::string WhoIsWith() const;

    static uint32_t constructed_counter;

private:
    std::shared_ptr<Y> y_{};  // 初期化状態では、y_はオブジェクトを所有しない(use_count()==0)
};

class Y final {
public:
    explicit Y() noexcept { ++constructed_counter; }
    ~Y() { --constructed_counter; }

    void Register(std::shared_ptr<X> x) { x_ = x; }

    std::shared_ptr<X> const& ref_x() const noexcept { return x_; }

    // 自身の状態を返す ("Y alone" または "Y with X")
    std::string WhoYouAre() const;

    // x_が保持するオブジェクトの状態を返す ("None" またはY::WhoYouAre()に委譲)
    std::string WhoIsWith() const;

    static uint32_t constructed_counter;

private:
    std::shared_ptr<X> x_{};  // 初期化状態では、x_はオブジェクトを所有しない(use_count()==0)
};

// Xのメンバ定義
std::string X::WhoYouAre() const { return y_ ? "X with Y" : "X alone"; }
std::string X::WhoIsWith() const { return y_ ? y_->WhoYouAre() : std::string{"None"}; }
uint32_t    X::constructed_counter;

// Yのメンバ定義
std::string Y::WhoYouAre() const { return x_ ? "Y with X" : "Y alone"; }
std::string Y::WhoIsWith() const { return x_ ? x_->WhoYouAre() : std::string{"None"}; }
uint32_t    Y::constructed_counter;
// @@@ sample end
#ifndef SANITIZER

TEST(WeakPtr, leak)
{
    // clang-format off
    // @@@ sample begin 2:1

    {
        ASSERT_EQ(X::constructed_counter, 0);
        ASSERT_EQ(Y::constructed_counter, 0);

        auto x0 = std::make_shared<X>();       // Xオブジェクトを持つshared_ptrの生成
        ASSERT_EQ(X::constructed_counter, 1);  // Xオブジェクトは1つ生成された

        ASSERT_EQ(x0.use_count(), 1);
        ASSERT_EQ(x0->WhoIsWith(), "None");     // x0.y_は何も保持していないので、"None"
        ASSERT_EQ(x0->ref_y().use_count(), 0);  // X::y_は何も持っていない

        // @@@ sample end
        // @@@ sample begin 2:2

        {
            auto y0 = std::make_shared<Y>();

            ASSERT_EQ(Y::constructed_counter, 1);   // Yオブジェクトは1つ生成された
            ASSERT_EQ(y0.use_count(), 1);
            ASSERT_EQ(y0->ref_x().use_count(), 0);  // y0.x_は何も持っていない
            ASSERT_EQ(y0->WhoYouAre(), "Y alone");  // y0.x_は何も持っていないので、"Y alone"

            x0->Register(y0);                       // これによりx0.y_はy0と同じYオブジェクトを持つ
            ASSERT_EQ(x0->WhoIsWith(), "Y alone");  // x0が持つYオブジェクトはまだXを持っていない状態

            y0->Register(x0);                       // これによりy0.y_はx0と同じオブジェクトを持つ
            // 上記で生成されたXオブジェクト、Yオブジェクトは、x0->Register(y0), y0->Register(x0)により
            // 相互に参照しあう状態になる
            ASSERT_EQ(X::constructed_counter, 1);   // 新しいオブジェクトが生成されるわけではない
            ASSERT_EQ(Y::constructed_counter, 1);   // 新しいオブジェクトが生成されるわけではない

            ASSERT_EQ(y0->WhoYouAre(), "Y with X"); // y0.x_はXオブジェクトを持っている
            ASSERT_EQ(x0->WhoYouAre(), "X with Y"); // x0.y_はYオブジェクトを持っている

            ASSERT_EQ(y0->WhoIsWith(), "X with Y"); // y0.x_はXオブジェクトを持っている
            ASSERT_EQ(x0->WhoIsWith(), "Y with X"); // x0.y_はYオブジェクトを持っている

            // 現時点で、x0とy0がお互いを持ち合う状態であることが確認できた
            // @@@ sample end
            // @@@ sample begin 2:3

            ASSERT_EQ(x0.use_count(), 2);           // x0、y0が相互に参照するので参照カウントが2に
            ASSERT_EQ(y0.use_count(), 2);           // x0、y0が相互に参照するので参照カウントが2に
            ASSERT_EQ(y0->ref_x().use_count(), 2);  // y0.x_の参照カウントは2
            ASSERT_EQ(x0->ref_y().use_count(), 2);  // x0.y_の参照カウントは2
        }  //ここでy0がスコープアウトするため、y0にはアクセスできないが、
           //x0を介して、y0が持っていたYオブジェクトにはアクセスできる

        ASSERT_EQ(x0->ref_y().use_count(), 1);  // y0がスコープアウトしたため、Yオブジェクトの参照カウントが減った
        ASSERT_EQ(x0->ref_y()->WhoYouAre(), "Y with X");  // x0.y_はXオブジェクトを持っている
        // @@@ sample end
        // @@@ sample begin 2:4
    }  // この次の行で、x0はスコープアウトするため、x0にはアクセスできず、すでにy0にもアクセスできない
    // ここではx0、y0がもともと持っていたXオブジェクト、Yオブジェクトへのポインタを完全に失ってしまった状態

    ASSERT_EQ(X::constructed_counter, 1);  // Xオブジェクトは未開放であり、リークが発生
    ASSERT_EQ(Y::constructed_counter, 1);  // Yオブジェクトは未開放であり、リークが発生
    // @@@ sample end
    // clang-format off
}
#endif
}  // namespace leak
