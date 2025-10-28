#include "gtest_wrapper.h"

namespace {
// @@@ sample begin 0:0

void lump()  // 実装によっては、ダイナミックメモリアロケーションをまとめらる場合がある
{
    int* p1 = new int{1};
    int* p2 = new int{2};
    int* p3 = new int{3};

    // 何らかの処理

    delete p1;
    delete p2;
    delete p3;

    // 上記のメモリアロケーションは、実装によっては下記のように最適化される場合がある

    int* p = new int[3]{1, 2, 3};
    // 何らかの処理

    delete[] p;
}

int emit()  // ダイナミックメモリアロケーションの省略
{
    int* p = new int{10};
    delete p;

    // 上記のメモリアロケーションは、下記の用にスタックの変数に置き換える最適化が許される

    int n = 10;

    return n;
}
// @@@ sample end
TEST(TermExplanation, heap_elision)
{
    lump();
    emit();
}
}  // namespace
