#include <cassert>
#include <cstdint>
#include <memory_resource>
#include <string>
#include <vector>

#include "gtest_wrapper.h"

#include "spin_lock.h"
#include "suppress_warning.h"
#include "utils.h"

SUPPRESS_WARN_BEGIN;
SUPPRESS_WARN_GCC_NOT_EFF_CPP;

namespace Inner_ {

struct header_t {
    header_t* next;
    size_t    n_nuits;  // header_tが何個あるか
};

constexpr auto unit_size = sizeof(header_t);

inline std::optional<header_t*> sprit(header_t* header, size_t n_nuits) noexcept
{
    assert(n_nuits > 1);  // ヘッダとバッファなので最低でも2

    if (header->n_nuits == n_nuits || header->n_nuits == n_nuits + 1) {
        return header->next;
    }
    else if (header->n_nuits > n_nuits) {
        auto next       = header + n_nuits;
        next->n_nuits   = header->n_nuits - n_nuits;
        next->next      = header->next;
        header->n_nuits = n_nuits;
        return next;
    }

    return std::nullopt;
}

inline void concat(header_t* front, header_t* rear) noexcept
{
    if (front + front->n_nuits == rear) {  // 1枚のメモリになる
        front->n_nuits += rear->n_nuits;
        front->next = rear->next;
    }
    else {
        front->next = rear;
    }
}

inline header_t* set_back(void* mem) noexcept { return static_cast<header_t*>(mem) - 1; }

static_assert(sizeof(header_t) == alignof(std::max_align_t));

template <uint32_t MEM_SIZE>
struct buffer_t {
    alignas(alignof(std::max_align_t)) uint8_t buffer[Roundup(sizeof(header_t), MEM_SIZE)];
};
}  // namespace Inner_

// @@@ sample begin 0:0

template <uint32_t MEM_SIZE>
class memory_resource_variable final : public std::pmr::memory_resource {
public:
    memory_resource_variable() noexcept
    {
        header_->next    = nullptr;
        header_->n_nuits = sizeof(buff_) / Inner_::unit_size;
    }

    size_t get_count() const noexcept { return unit_count_ * Inner_::unit_size; }
    bool   is_valid(void const* mem) const noexcept
    {
        return (&buff_ < mem) && (mem < &buff_.buffer[ArrayLength(buff_.buffer)]);
    }

    // @@@ ignore begin
    class const_iterator {
    public:
        explicit const_iterator(Inner_::header_t const* header) noexcept : header_{header} {}
        const_iterator(const_iterator const&) = default;
        const_iterator(const_iterator&&)      = default;

        const_iterator& operator++() noexcept  // 前置++のみ実装
        {
            assert(header_ != nullptr);
            header_ = header_->next;

            return *this;
        }

        Inner_::header_t const* operator*() noexcept { return header_; }

        // clang-format off

    #if __cplusplus <= 201703L  // c++17
        bool operator==(const_iterator const& rhs) noexcept { return header_ == rhs.header_; }
        bool operator!=(const_iterator const& rhs) noexcept { return !(*this == rhs); }
    #else  // c++20

        auto operator<=>(const const_iterator&) const = default;
    #endif
        // clang-format on

    private:
        Inner_::header_t const* header_;
    };

    const_iterator begin() const noexcept { return const_iterator{header_}; }
    const_iterator end() const noexcept { return const_iterator{nullptr}; }
    const_iterator cbegin() const noexcept { return const_iterator{header_}; }
    const_iterator cend() const noexcept { return const_iterator{nullptr}; }
    // @@@ ignore end

private:
    using header_t = Inner_::header_t;

    Inner_::buffer_t<MEM_SIZE> buff_{};
    header_t*                  header_{reinterpret_cast<header_t*>(buff_.buffer)};
    mutable SpinLock           spin_lock_{};
    size_t                     unit_count_{sizeof(buff_) / Inner_::unit_size};
    size_t                     unit_count_min_{sizeof(buff_) / Inner_::unit_size};

    void* do_allocate(size_t size, size_t) override
    {
        // MPoolVariable::allocとほぼ同じ
        // @@@ ignore begin
        auto n_nuits = (Roundup(Inner_::unit_size, size) / Inner_::unit_size) + 1;

        auto lock = std::lock_guard{spin_lock_};

        auto curr = header_;

        for (header_t* prev{nullptr}; curr != nullptr; prev = curr, curr = curr->next) {
            auto opt_next = std::optional<header_t*>{sprit(curr, n_nuits)};

            if (!opt_next) {
                continue;
            }

            auto next = *opt_next;
            if (prev == nullptr) {
                header_ = next;
            }
            else {
                prev->next = next;
            }
            break;
        }

        if (curr != nullptr) {
            unit_count_ -= curr->n_nuits;
            unit_count_min_ = std::min(unit_count_, unit_count_min_);
            ++curr;
        }

        if (curr == nullptr) {
            throw std::bad_alloc{};
        }

        return curr;
        // @@@ ignore end
    }

    void do_deallocate(void* mem, size_t, size_t) noexcept override
    {
        // MPoolVariable::freeとほぼ同じ
        // @@@ ignore begin
        header_t* to_free = Inner_::set_back(mem);

        to_free->next = nullptr;

        auto lock = std::lock_guard{spin_lock_};

        unit_count_ += to_free->n_nuits;
        unit_count_min_ = std::min(unit_count_, unit_count_min_);

        if (header_ == nullptr) {
            header_ = to_free;
            return;
        }

        if (to_free < header_) {
            concat(to_free, header_);
            header_ = to_free;
            return;
        }

        header_t* curr = header_;

        for (; curr->next != nullptr; curr = curr->next) {
            if (to_free < curr->next) {  // 常に curr < to_free
                concat(to_free, curr->next);
                concat(curr, to_free);
                return;
            }
        }

        concat(curr, to_free);
        // @@@ ignore end
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }
};
// @@@ sample end

namespace {
TEST(NewDelete, pmr_memory_resource)
{
    // @@@ sample begin 1:0

    constexpr uint32_t            max = 1024;
    memory_resource_variable<max> mrv;
    memory_resource_variable<max> mrv2;

    ASSERT_EQ(mrv, mrv);
    ASSERT_NE(mrv, mrv2);

    {
        auto remaings1 = mrv.get_count();

        ASSERT_GE(max, remaings1);

        // std::basic_stringにカスタムアロケータを適用
        using pmr_string = std::basic_string<char, std::char_traits<char>,
                                             std::pmr::polymorphic_allocator<char>>;
        std::pmr::polymorphic_allocator<char> allocator(&mrv);

        // カスタムアロケータを使って文字列を作成
        pmr_string str("custom allocator!", allocator);
        auto       remaings2 = mrv.get_count();
        // アサーション: 文字列の内容を確認

        ASSERT_GT(remaings1, remaings2);
        ASSERT_EQ("custom allocator!", str);

        ASSERT_TRUE(mrv.is_valid(str.c_str()));  // strの内部メモリがmrvの内部であることの確認

        auto str3 = str + str + str;
        ASSERT_EQ(str.size() * 3 + 1, str3.size() + 1);
        ASSERT_THROW(str3 = pmr_string(2000, 'a'), std::bad_alloc);  // メモリの枯渇テスト
    }

    ASSERT_GE(max, mrv.get_count());  // 解放後のメモリの回復のテスト
    // @@@ sample end
}
}  // namespace
