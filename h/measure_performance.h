#pragma once
#include <chrono>

// @@@ sample begin 0:0

/// @brief 関数fをount回実行し、その実行時間を計測する。
/// @tparam FUNC 実行する関数オブジェクトの型
/// @param count 関数を実行する回数
/// @param f 実行する関数オブジェクト
/// @return 実行時間
template <typename FUNC>
std::chrono::milliseconds MeasurePerformance(uint32_t count, FUNC f)
{
    auto const start = std::chrono::system_clock::now();

    for (auto i = 0U; i < count; ++i) {
        f();
    }

    auto const stop = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
}
// @@@ sample end
