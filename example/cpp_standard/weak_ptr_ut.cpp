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

    std::weak_ptr<X> const& ref_x() const noexcept { return x_; }

    // 自身の状態を返す ("Y alone" または "Y with X")
    std::string WhoYouAre() const;

    // x_が保持するオブジェクトの状態を返す ("None" またはY::WhoYouAre()に委譲)
    std::string WhoIsWith() const;

    static uint32_t constructed_counter;

private:
    std::weak_ptr<X> x_{};
};

// Xのメンバ定義
std::string X::WhoYouAre() const { return y_ ? "X with Y" : "X alone"; }
std::string X::WhoIsWith() const { return y_ ? y_->WhoYouAre() : std::string{"None"}; }
uint32_t    X::constructed_counter;

// Yのメンバ定義
std::string Y::WhoYouAre() const { return x_.use_count() != 0 ? "Y with X" : "Y alone"; }
// 注: weak_ptrはbool変換をサポートしないため、use_count() != 0 で有効性を判定
// @@@ sample end
// @@@ sample begin 0:1
std::string Y::WhoIsWith() const  // 修正版Y::WhoIsWithの定義
{
    if (auto x = x_.lock(); x) {  // Xオブジェクトが解放されていた場合、xはstd::shared_ptr<X>{}となり、falseと評価される
        return x->WhoYouAre();
    }
    else {
        return "None";
    }
}
// @@@ sample end
// @@@ sample begin 0:2
uint32_t Y::constructed_counter;
// @@@ sample end
#ifndef SANITIZER

TEST(WeakPtr, not_leak_by_weak)
{
    // clang-format off
    // @@@ sample begin 0:3

    {
        ASSERT_EQ(X::constructed_counter, 0);
        ASSERT_EQ(Y::constructed_counter, 0);

        auto x0 = std::make_shared<X>();       // Xオブジェクトを持つshared_ptrの生成
        ASSERT_EQ(X::constructed_counter, 1);  // Xオブジェクトは1つ生成された

        ASSERT_EQ(x0.use_count(), 1);
        ASSERT_EQ(x0->WhoYouAre(), "X alone");  // x0.y_は何も保持していないので、"X alone"
        ASSERT_EQ(x0->ref_y().use_count(), 0);  // X::y_は何も持っていない

        {
            auto y0 = std::make_shared<Y>();

            ASSERT_EQ(Y::constructed_counter, 1);       // Yオブジェクトは1つ生成された
            ASSERT_EQ(y0.use_count(), 1);
            ASSERT_EQ(y0->ref_x().use_count(), 0);      // y0.x_は何も持っていない
            ASSERT_EQ(y0->WhoYouAre(), "Y alone");      // y0.x_は何も持っていないので、"Y alone"

            x0->Register(y0);                           // これによりx0.y_はy0と同じオブジェクトを持つ
            ASSERT_EQ(x0->WhoYouAre(), "X with Y");     // x0.y_はYオブジェクトを持っている

            y0->Register(x0);  // これによりy0.x_はx0と同じXオブジェクトを持つことができる
            ASSERT_EQ(y0->WhoIsWith(), "X with Y");     // y0.x_が持っているXオブジェクトはYを持っている
            
            // x0->Register(y0), y0->Register(x0)により Xオブジェクト、Yオブジェクトは相互参照できる状態となった
            ASSERT_EQ(X::constructed_counter, 1);       // 新しいオブジェクトが生成されるわけではない
            ASSERT_EQ(Y::constructed_counter, 1);       // 新しいオブジェクトが生成されるわけではない

            ASSERT_EQ(y0->WhoYouAre(), "Y with X");     // y0.x_はXオブジェクトを持っている
            ASSERT_EQ(x0->WhoYouAre(), "X with Y");     // x0.y_はYオブジェクトを持っている(再確認)
            ASSERT_EQ(y0->WhoIsWith(), "X with Y");     // y0が参照するXオブジェクトはYを持っている
            // 現時点で、x0とy0がお互いを相互参照できることが確認できた

            // weak_ptrを使用した効果によりXオブジェクトの参照カウントは増加しない
            ASSERT_EQ(x0.use_count(), 1);               // y0.x_はweak_ptrなので参照カウントに影響しない
            ASSERT_EQ(y0.use_count(), 2);               // x0.y_はshared_ptrなので参照カウントが2
            ASSERT_EQ(y0->ref_x().use_count(), 1);      // y0.x_の参照カウントは1
            ASSERT_EQ(x0->ref_y().use_count(), 2);      // x0.y_の参照カウントは2
        }  //ここでy0がスコープアウトするため、y0にはアクセスできないが、
           // x0を介して、y0が持っていたYオブジェクトにはアクセスできる

        ASSERT_EQ(x0->ref_y().use_count(), 1);  // y0がスコープアウトしたため、Yオブジェクトの参照カウントが減った
        ASSERT_EQ(x0->ref_y()->WhoYouAre(), "Y with X");  // x0.y_はXオブジェクトを持っている
    }  // この次の行で、x0はスコープアウトし、以下の処理が実行される:
       //   1. x0のデストラクタが呼ばれ、x0.y_の参照カウントがデクリメント
       //   2. x0.y_の参照カウントが1→0になり、保持していたYオブジェクトを解放する
       //   3. Yオブジェクトのデストラクタ内でy_.x_(weak_ptr)が破棄されるが、weak_ptrなのでXオブジェクトの参照カウントには影響しない
       //   4. x0本体のデストラクタが完了し、Xオブジェクトの参照カウントが1→0になり、Xオブジェクトも解放される

    // 上記1-4によりダイナミックに生成されたオブジェクトは解放されたため、下記のテストが成立する
    ASSERT_EQ(X::constructed_counter, 0);
    ASSERT_EQ(Y::constructed_counter, 0);
    // @@@ sample end
    // clang-format on
}
#endif
}  // namespace not_leak_by_weak
