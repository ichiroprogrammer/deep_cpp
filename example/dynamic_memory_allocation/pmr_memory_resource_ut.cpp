#include <cassert>
#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "spin_lock.h"
#include "suppress_warning.h"

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_GCC_NOT_EFF_CPP;
// 既存のMPoolVariableを拡張する
template <uint32_t MEM_SIZE>
class MPoolVariableMemoryResource final : public std::pmr::memory_resource {
private:
    struct header_t {
        header_t* next;
        size_t    n_units;
    };

    struct buffer_t {
        alignas(std::max_align_t) char buffer[MEM_SIZE];
    };

    buffer_t         buff_{};
    header_t*        header_{reinterpret_cast<header_t*>(buff_.buffer)};
    size_t           unit_count_{MEM_SIZE / sizeof(header_t)};
    mutable SpinLock spin_lock_{};

protected:
    void* do_allocate(size_t bytes, size_t) override
    {
        std::lock_guard lock{spin_lock_};

        // メモリ不足の確認
        if (bytes > (unit_count_ * sizeof(header_t))) {
            throw std::bad_alloc{};
        }

        if (!header_) {
            throw std::bad_alloc{};
        }

        header_t* result = header_;
        header_          = header_->next;
        unit_count_--;
        return static_cast<void*>(result);
    }

    void do_deallocate(void* p, size_t, size_t) override
    {
        std::lock_guard lock{spin_lock_};

        auto* to_free = static_cast<header_t*>(p);
        to_free->next = header_;
        header_       = to_free;
        unit_count_++;
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
    {
        return this == &other;
    }

public:
    MPoolVariableMemoryResource()
    {
        header_ = reinterpret_cast<header_t*>(buff_.buffer);
        for (size_t i = 0; i < unit_count_ - 1; ++i) {
            auto* current = reinterpret_cast<header_t*>(&buff_.buffer[i * sizeof(header_t)]);
            current->next = reinterpret_cast<header_t*>(&buff_.buffer[(i + 1) * sizeof(header_t)]);
        }
        reinterpret_cast<header_t*>(&buff_.buffer[(unit_count_ - 1) * sizeof(header_t)])->next
            = nullptr;
    }

    static constexpr size_t SizeOfUnit = sizeof(header_t);

    // 現在の残りユニット数を取得する
    size_t get_remaining_units() const noexcept { return unit_count_; }

    // 現在のユニットサイズを取得する
    size_t get_unit_size() const noexcept { return sizeof(header_t); }
};
SUPPRESS_WARN_END;

namespace {
TEST(NewDelete, pmr_memory_resource)
{
    // @@@ sample begin 0:0

    MPoolVariableMemoryResource<1024> allocator;
    // @@@ sample end
}
}  // namespace

TEST(NewDelete, pmr_memory_resource_0)
{
    MPoolVariableMemoryResource<1024> resource;

    // 初期の残りユニット数を確認
    size_t initial_units = resource.get_remaining_units();
    ASSERT_EQ(initial_units, 1024 / MPoolVariableMemoryResource<1024>::SizeOfUnit);

#if 0 
    // pmr::vectorを使用してメモリを確保
    std::pmr::vector<int> vec{&resource};
    vec.push_back(42);

    // 残りユニット数が減少したことを確認
    ASSERT_LT(resource.get_remaining_units(), initial_units);

    // メモリ解放後の残りユニット数を確認
    vec.clear();
    vec.shrink_to_fit(); // メモリを解放
    ASSERT_EQ(resource.get_remaining_units(), initial_units);
#endif
}

TEST(NewDelete, pmr_memory_resource_1)
{
#if 0
    MPoolVariableMemoryResource<2048> resource;

    // 初期の残りユニット数を確認
    size_t initial_units = resource.get_remaining_units();

    // pmr::stringを使用してメモリを確保
    std::pmr::string str("Test String", &resource);

    // 残りユニット数が減少したことを確認
    ASSERT_LT(resource.get_remaining_units(), initial_units);

    // メモリ解放後の残りユニット数を確認
    str.clear();
    ASSERT_EQ(resource.get_remaining_units(), initial_units);
#endif
}
