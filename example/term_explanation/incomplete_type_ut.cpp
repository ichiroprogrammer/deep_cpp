#include "gtest_wrapper.h"

namespace {
// @@@ sample begin 0:0

template <typename T, typename = void>
struct is_complete : std::false_type {
};

template <typename T>  // sizeof(T) が有効であれば、Tは完全型であると判定
struct is_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {
};

template <typename T>
constexpr bool is_complete_v = is_complete<T>::value;
// @@@ sample end

TEST(IncompleteType, incomplete_type)
{
    {
        // @@@ sample begin 1:0

        class A;  // Aの前方宣言
                  // これ以降、Aは不完全型となる

        // auto a = sizeof(A);  Aが不完全型であるため、コンパイルエラー
        static_assert(!is_complete_v<A>);
        // @@@ sample end
    }
    {
        // @@@ sample begin 1:1

        class A {  // この宣言により、この行以降はAは完全型になる
        public:
            // 何らかの宣言
        };

        auto a = sizeof(A);  // Aが完全型であるため、コンパイル可能
        static_assert(is_complete_v<A>);
        // @@@ sample end
        ASSERT_EQ(1, a);
    }
}
}  // namespace
