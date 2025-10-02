#include "gtest_wrapper.h"

#include "light_pimpl.h"

// @@@ sample begin 0:0

namespace {
bool constructor_called = false;  // テスト用フラグであるため、実装には無関係
bool destructor_called  = false;  // テスト用フラグであるため、実装には無関係
}  // namespace

struct LightPimpl::Impl_t {
    Impl_t(std::string const& name) : name_{name} { constructor_called = true; /* コンストラクタが呼ばれたマーク */ }
    std::vector<uint8_t> vect_{};
    std::string          name_{};
    uint8_t              buffer_[BufferLen]{};

    ~Impl_t() { destructor_called = true; /* デストラクタが呼ばれたマーク */ }
};
// @@@ sample end
// @@@ sample begin 0:1

LightPimpl::LightPimpl(std::string const& name) : pimpl_(new (memory_) Impl_t{name})  // プレースメントnew
{
    static_assert(sizeof(Impl_t) <= sizeof(memory_), "Buffer size mismatch");
    static_assert(sizeof(memory_) - sizeof(Impl_t) < 16, "Buffer has excessive padding");
    static_assert(alignof(Impl_t) <= alignof(std::max_align_t), "Buffer alignment mismatch");
}
// @@@ sample end
// @@@ sample begin 0:2

LightPimpl::~LightPimpl() { pimpl_->~Impl_t(); }  // Impl_tのデストラクタを直接呼び出す
// @@@ sample end
// @@@ sample begin 0:3

std::vector<uint8_t> const& LightPimpl::GetVector() const { return pimpl_->vect_; }
std::string const&          LightPimpl::GetName() const { return pimpl_->name_; }

uint8_t (&LightPimpl::GetBuffer())[LightPimpl::BufferLen] { return pimpl_->buffer_; }

uint8_t const (&LightPimpl::GetBuffer() const)[LightPimpl::BufferLen] { return pimpl_->buffer_; }
// @@@ sample end

namespace {

TEST(Pimpl, light_weight)
{
    // @@@ sample begin 1:0

    {
        ASSERT_FALSE(constructor_called);
        LightPimpl lp{"lp"};

        ASSERT_EQ(lp.GetName(), "lp");
        ASSERT_EQ(lp.GetVector().size(), 0);
        ASSERT_EQ(lp.GetBuffer()[0], 0x0);

        ASSERT_TRUE(constructor_called);
        ASSERT_FALSE(destructor_called);
    };  // この行で、lpは解放される

    ASSERT_TRUE(destructor_called);
    // @@@ sample end
}
}  // namespace
