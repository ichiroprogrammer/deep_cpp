#include <type_traits>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace Nstd {

// @@@ sample begin 0:0

template <typename T>  // std::is_povはC++20から非推奨
constexpr bool is_pod_v = std::is_trivial_v<T>&& std::is_standard_layout_v<T>;
// @@@ sample end

TEST(Pod, pod)
{
    {
        // @@@ sample begin 0:1

        static_assert(is_pod_v<int>);
        static_assert(is_pod_v<int const>);
        static_assert(is_pod_v<int*>);
        static_assert(is_pod_v<int[3]>);
        static_assert(!is_pod_v<int&>);  // リファレンスはPODではない

        struct Pod {};

        static_assert(is_pod_v<Pod>);
        static_assert(is_pod_v<Pod const>);
        static_assert(is_pod_v<Pod*>);
        static_assert(is_pod_v<Pod[3]>);
        static_assert(!is_pod_v<Pod&>);

        struct NonPod {  // コンストラクタがあるためPODではない
            NonPod();
        };

        static_assert(!is_pod_v<NonPod>);
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:2

        static_assert(std::is_standard_layout_v<int>);
        static_assert(std::is_standard_layout_v<int*>);
        static_assert(std::is_standard_layout_v<int[1]>);
        static_assert(!std::is_standard_layout_v<int&>);

        enum class SizeUndefined { su_0, su_1 };

        struct StanderdLayout {  // 標準レイアウトだがトリビアルではない
            StanderdLayout() : a{0}, b{SizeUndefined::su_0} {}
            int           a;
            SizeUndefined b;
        };

        static_assert(std::is_standard_layout_v<StanderdLayout>);
        static_assert(!std::is_trivial_v<StanderdLayout>);
        static_assert(!is_pod_v<StanderdLayout>);
        // @@@ sample end
    }
    {
        // @@@ sample begin 0:3

        static_assert(std::is_trivial_v<int>);
        static_assert(std::is_trivial_v<int*>);
        static_assert(std::is_trivial_v<int[1]>);
        static_assert(!std::is_trivial_v<int&>);

        enum class SizeUndefined { su_0, su_1 };

        struct Trivial {      // トリビアルだが標準レイアウトではない
            int&          a;  // リファレンスは標準レイアウトではない
            SizeUndefined b;
        };

        static_assert(!std::is_standard_layout_v<Trivial>);
        static_assert(std::is_trivial_v<Trivial>);
        static_assert(!is_pod_v<Trivial>);
        // @@@ sample end
    }

    {
        // @@@ sample begin 1:0

        static_assert(std::is_trivially_destructible_v<int>);
        static_assert(std::is_trivially_destructible_v<int*>);
        static_assert(std::is_trivially_destructible_v<int[1]>);
        static_assert(std::is_trivially_destructible_v<int&>);

        enum class SizeUndefined { su_0, su_1 };

        struct Trivial {      // トリビアルに破壊可能でないため、トリビアル型ではない
            int           a;
            SizeUndefined b;
            ~Trivial(){}
        };

        static_assert(std::is_standard_layout_v<Trivial>);
        static_assert(!std::is_trivial_v<Trivial>);
        static_assert(!std::is_trivially_destructible_v<Trivial>);
        // @@@ sample end
    }
}
}  // namespace Nstd
