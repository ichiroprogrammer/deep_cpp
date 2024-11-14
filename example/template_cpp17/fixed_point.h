#pragma once

#include <compare>
#include <cstdint>
#include <type_traits>

// @@@ sample begin 0:0

namespace Nstd {
/// @brief BASIC_TYPEで指定する基本型のビット長を持つ固定小数点を扱うためのクラス
/// @tparam BASIC_TYPE       全体のビット長や、符号を指定するための整数型
/// @tparam FRACTION_BIT_NUM 小数点保持のためのビット長
template <typename BASIC_TYPE, uint32_t FRACTION_BIT_NUM>
class FixedPoint {
public:
    constexpr FixedPoint(BASIC_TYPE                                integer  = 0,
                         typename std::make_unsigned_t<BASIC_TYPE> fraction = 0) noexcept
        : value_{get_init_value(integer, fraction)}
    {
        // signedに対する右ビットシフトの仕様が、算術右ビットシフトでないと
        // このクラスは成立しない。下記のstatic_assertはその確認。
        static_assert(IsSigned() ? (-1 >> 1 == -1) : true, "need logical right bit sift");

        // BASIC_TYPEをcharにすることは認めない。
        static_assert(!std::is_same_v<BASIC_TYPE, char>, "BASIC_TYPE should not be char");
    }

    // 以下、特殊メンバ定義
    ~FixedPoint()                                = default;
    FixedPoint(FixedPoint const&)                = default;
    FixedPoint& operator=(FixedPoint const&)     = default;
    FixedPoint(FixedPoint&&) noexcept            = default;
    FixedPoint& operator=(FixedPoint&&) noexcept = default;

    // 以下、インターフェース定義
    constexpr BASIC_TYPE GetValue() const noexcept { return value_; }
    constexpr BASIC_TYPE GetInteger() const noexcept { return value_ >> fraction_bit_num_; }
    constexpr BASIC_TYPE GetFraction() const noexcept { return value_ & fraction_bit_mask_; }

    /// @brief doubleに変換する
    constexpr double ToFloatPoint() const noexcept
    {
        return GetInteger() + (static_cast<double>(GetFraction()) / (fraction_bit_mask_ + 1));
    }

    /// @brief 小数部のビット長を返す
    constexpr typename std::make_unsigned_t<BASIC_TYPE> GetFractionMask() const noexcept
    {
        return fraction_bit_mask_;
    }

    /// @brief 整数部のビット長を返す
    constexpr typename std::make_unsigned_t<BASIC_TYPE> GetIntegerMask() const noexcept
    {
        return integer_bit_mask_;
    }

    static constexpr bool IsSigned() noexcept { return std::is_signed_v<BASIC_TYPE>; }

    static constexpr bool IsUnsigned() noexcept { return std::is_unsigned_v<BASIC_TYPE>; }

    /// @brief    以下operator @=の定義
    FixedPoint& operator+=(FixedPoint rhs) noexcept
    {
        value_ += rhs.value_;
        return *this;
    }

    FixedPoint& operator-=(FixedPoint rhs) noexcept
    {
        value_ -= rhs.value_;
        return *this;
    }

    FixedPoint& operator*=(FixedPoint rhs) noexcept
    {
        value_ *= rhs.value_ >> fraction_bit_num_;
        return *this;
    }

    FixedPoint& operator/=(FixedPoint rhs) noexcept
    {
        using T = std::conditional_t<IsSigned(), int64_t, uint64_t>;

        value_ = (static_cast<T>(value_) << fraction_bit_num_) / rhs.value_;

        return *this;
    }

private:
    BASIC_TYPE value_;  // FixedPointの実際の値

    static constexpr uint32_t bit_mask(uint32_t bit_len) noexcept
    {
        if (bit_len == 0) {
            return 0x0;
        }

        return bit_mask(bit_len - 1) | (0x01 << (bit_len - 1));
    }

    static constexpr uint32_t fraction_bit_num_{FRACTION_BIT_NUM};
    static constexpr uint32_t fraction_bit_mask_{bit_mask(fraction_bit_num_)};
    static constexpr uint32_t integer_bit_num_{sizeof(BASIC_TYPE) * 8 - FRACTION_BIT_NUM};
    static constexpr uint32_t integer_bit_mask_{bit_mask(integer_bit_num_) << fraction_bit_num_};

    static constexpr BASIC_TYPE get_init_value(BASIC_TYPE integer, BASIC_TYPE fraction) noexcept
    {
        // 本来は左シフト<<を使いたいが、signedに対しての<<ランタイム実装依存であるので、
        // return (integer << fraction_bit_num_) | fraction;

        return (integer * (fraction_bit_mask_ + 1)) | fraction;
    }

    /// 以下比較演算子の定義

#if __cplusplus >= 202002L  // C++20
    friend auto operator<=>(FixedPoint lhs, FixedPoint rhs) noexcept = default;
#else  // C++17
    friend bool operator==(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        return lhs.value_ == rhs.value_;
    }

    friend bool operator!=(FixedPoint lhs, FixedPoint rhs) noexcept { return !(lhs == rhs); }

    friend bool operator>(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        return lhs.value_ > rhs.value_;
    }

    friend bool operator>=(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        return (lhs > rhs) || (lhs == rhs);
    }

    friend bool operator<(FixedPoint lhs, FixedPoint rhs) noexcept { return (rhs > lhs); }

    friend bool operator<=(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        return (lhs < rhs) || (lhs == rhs);
    }
#endif

    /// @brief FixedPoint() + intのようなオーバーロードを作るためにあえてfriend
    friend FixedPoint operator+(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    friend FixedPoint operator-(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    friend FixedPoint operator*(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        lhs *= rhs;
        return lhs;
    }

    friend FixedPoint operator/(FixedPoint lhs, FixedPoint rhs) noexcept
    {
        lhs /= rhs;
        return lhs;
    }
};
// @@@ sample end

}  // namespace Nstd

// @@@ sample begin 0:1

namespace Nstd {
namespace fixed_point_literals {
// FixedPoint<int32_t, 8> 用のユーザ定義リテラル
FixedPoint<int32_t, 8> operator"" _fxp(unsigned long long int val)
{
    // 整数部分を取り、FixedPoint<int32_t, 8>のインスタンスを作成
    return FixedPoint<int32_t, 8>(static_cast<int32_t>(val));
}

FixedPoint<int32_t, 8> operator"" _fxp(long double val)
{
    // 浮動小数点数からFixedPoint<int32_t, 8>のインスタンスを作成
    int32_t integer_part  = static_cast<int32_t>(val);
    int32_t fraction_part = static_cast<int32_t>((val - integer_part) * 256);  // 2^8 = 256
    return FixedPoint<int32_t, 8>(integer_part, fraction_part);
}
}  // namespace fixed_point_literals
}  // namespace Nstd
// @@@ sample end
