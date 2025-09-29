#include <memory_resource>
#include <thread>
#include <vector>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

struct Point {
    int  x;
    int  y;
    bool operator==(Point const& rhs) const noexcept = default;
};

class Circl {
public:
    Circl(Point center, uint32_t radius) : center_{center}, radius_{radius} {}

    std::string to_string()
    {
        std::ostringstream oss;

        oss << "center x:" << center_.x << " y:" << center_.y << " radius:" << radius_;
        return oss.str();
    }

    bool operator==(Circl const& rhs) const noexcept = default;

private:
    Point const center_;
    uint32_t    radius_;
};
// @@@ sample end

TEST(ExpTerm, designated_init)
{
    // @@@ sample begin 0:1

    struct Point p0 {
        10, 20
    };
    struct Point p1 {
        .x = 10, .y = 20
    };  // x、yを明示できるため、可読性向上が見込める

    ASSERT_EQ(p0, p1);

    Circl circl_0{p1, 2U};
    ASSERT_EQ("center x:10 y:20 radius:2", circl_0.to_string());

    Circl circl_1{{10, 20}, 2U};  // circl_2に比べると可読性に劣る
    ASSERT_EQ("center x:10 y:20 radius:2", circl_1.to_string());

    Circl circl_2{{.x = 10, .y = 20}, 2U};  // x、yを明示できるため、可読性向上が見込める
    ASSERT_EQ("center x:10 y:20 radius:2", circl_2.to_string());

    ASSERT_EQ(circl_1, circl_2);
    // @@@ sample end
}

TEST(ExpTerm, unsynchronized_pool_resource)
{
    {
        // @@@ sample begin 1:0

        std::pmr::unsynchronized_pool_resource pool_resource(
            std::pmr::pool_options{
                .max_blocks_per_chunk        = 10,   // チャンクあたりの最大ブロック数
                .largest_required_pool_block = 1024  // 最大ブロックサイズ
            },
            std::pmr::new_delete_resource()  // フォールバックリソース
        );

        std::pmr::vector<int> vec{&pool_resource};  // pmrを使用するベクタの定義
        // @@@ sample end
    }

    {
        // @@@ sample begin 1:1

        // 指示付き初期化を使わずにstd::pmr::unsynchronized_pool_resourceの初期化
        std::pmr::unsynchronized_pool_resource pool_resource(
            std::pmr::pool_options{
                10,   // チャンクあたりの最大ブロック数
                1024  // 最大ブロックサイズ
            },
            std::pmr::new_delete_resource()  // フォールバックリソース
        );

        std::pmr::vector<int> vec{&pool_resource};  // pmrを使用するベクタの定義
        // @@@ sample end
    }
}
}  // namespace
