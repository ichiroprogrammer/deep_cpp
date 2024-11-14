#pragma once
#include <concepts>
#include <functional>

namespace Nstd {

// @@@ sample begin 0:0

/// @brief RAIIのためのクラス。コンストラクタ引数の関数オブジェクトをデストラクタから呼び出す
///
// clang-format off
#if __cplusplus >= 202002L   // c++20

template <std::invocable F>  // Fが呼び出し可能であることを制約
#else  // c++17

template <typename F>
#endif
// clang-format on
// @@@ sample end
// @@@ sample begin 0:1
class ScopedGuard {
public:
    explicit ScopedGuard(F&& f) noexcept : f_{f}
    {
        // @@@ sample end
        // @@@ sample begin 0:2
    }

    ~ScopedGuard() { f_(); }
    ScopedGuard(ScopedGuard const&)            = delete;  // copyは禁止
    ScopedGuard& operator=(ScopedGuard const&) = delete;  // copyは禁止

private:
    F f_;
};
// @@@ sample end
// @@@ sample begin 0:3

template <typename F>
ScopedGuard<F> MakeScopedGuard(F&& f) noexcept
{
    return ScopedGuard<F>(std::move(f));
}
// @@@ sample end

}  // namespace Nstd
