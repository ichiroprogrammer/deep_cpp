#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "gtest_wrapper.h"

namespace CRTP {
// @@@ sample begin 0:0

template <typename T>
class Base {
    // @@@ ignore begin
    // @@@ ignore end
};

class Derived : public Base<Derived> {
    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end
// @@@ sample begin 1:0

std::atomic<uint32_t> DerivedClass_Count = 0;  // Counterの派生クラスのインスタンス数

template <typename T>
class Counter {  // 派生クラスのインスタンスを計測するミックスイン
public:
    Counter() { ++DerivedClass_Count; }
    Counter(Counter const&) { ++DerivedClass_Count; }
    ~Counter() { --DerivedClass_Count; }
};

class A : public Counter<A> {
    // クラスAの実装
    // ...
};

class B : public Counter<B> {
    // クラスBの実装
    // ...
};
// @@@ sample end

TEST(CRTP, crpt)
{
    // @@@ sample begin 1:1

    A a0;
    B b0;

    ASSERT_EQ(2, DerivedClass_Count);  // インスタンスが２個のテスト

    {
        auto a1 = a0;
        ASSERT_EQ(3, DerivedClass_Count);  // コピーによりインスタンスの増加のテスト
    }
    ASSERT_EQ(2, DerivedClass_Count);  // a1のスコープアウトによりインスタンスが減少
    // @@@ sample end
}
}  // namespace CRTP
