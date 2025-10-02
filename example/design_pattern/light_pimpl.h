#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

// @@@ sample begin 0:0

class LightPimpl {
public:
    LightPimpl(std::string const& name);
    ~LightPimpl();
    LightPimpl(LightPimpl const&)            = delete;  // コピーは禁止
    LightPimpl& operator=(LightPimpl const&) = delete;  // コピーは禁止

    std::vector<uint8_t> const& GetVector() const;
    std::string const&          GetName() const;

    static constexpr size_t BufferLen = 10;
    uint8_t (&GetBuffer())[BufferLen];
    uint8_t const (&GetBuffer() const)[BufferLen];  // const を追加

private:
    // Impl_tをプレースメントnewでインスタンス化する時に使用するメモリ
    alignas(std::max_align_t) uint8_t memory_[48];  // 配列長はsizeof(Impl_t)以上になるよう調整
    struct Impl_t;
    struct Impl_t* pimpl_;
};
// @@@ sample end
