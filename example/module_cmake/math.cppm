// @@@ sample begin 0:0

export module math;  // モジュール宣言

namespace math {                                // 通常の名前空間。モジュール名と同じにして良い
export int add(int a, int b) { return a + b; }  // 外部公開シンボル
export int multiply(int a, int b) { return a * b; }  // 外部公開シンボル
}  // namespace math
// @@@ sample end
