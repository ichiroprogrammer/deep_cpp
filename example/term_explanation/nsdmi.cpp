#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace nadmi {
SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_CLANG_UNUSED_PRIVATE_FIELD;
SUPPRESS_WARN_GCC_NOT_EFF_CPP;
// @@@ sample begin 0:0

class A {
public:
    A() : a_{1}  // NSDMIではなく、非静的なメンバ初期化子による初期化
    {
    }

private:
    int32_t     a_;
    int32_t     b_ = 0;        // NSDMI
    std::string str_{"init"};  // NSDMI
};
// @@@ sample end
}  // namespace nadmi
namespace nadmi2 {
// @@@ sample begin 0:1

class A {
public:
    A(int a, int b) : a_{a}, v_{a, b, 3}  // 非静的なメンバ初期化子による初期化
    {
    }

private:
    int              a_;
    std::vector<int> v_;
};
// @@@ sample end
}  // namespace nadmi2
namespace nadmi3 {
// @@@ sample begin 0:2

class A {
public:
    A(int a, int b)
    {
        a_ = b;                     // 非静的なメンバのコンストラクタでの代入
        v_ = std::vector{a, b, 3};  // 非静的なメンバのコンストラクタでの代入
    }

private:
    int              a_;
    std::vector<int> v_;
};
// @@@ sample end
SUPPRESS_WARN_END;
}  // namespace nadmi3
