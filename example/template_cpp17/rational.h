#pragma once

#include <concepts>
#include <cstdint>
#include <istream>
#include <numeric>
#include <string>
#include <string_view>

// @@@ sample begin 0:0

namespace Nstd {
/// @brief ユーザー指定の型で分数を扱うためのクラス
/// @tparam T 基本の整数型（デフォルトはint32_t）
#if __cplusplus == 202002L  // c++20
template <std::signed_integral T = int32_t>
#else
template <typename T = int32_t>
#endif
class Rational {
public:
    using underlying_type_t = T;
    /// @brief コンストラクタ
    /// @param num 分子
    /// @param deno 分母
    constexpr Rational(T num, T deno = 1) : value_{reduce(num, deno)} {}
    T getNumerator() const noexcept { return value_.num; }
    T getDenominator() const noexcept { return value_.deno; }
    /// @brief 2項演算子(四則演算)の定義
    Rational operator+(const Rational& rhs) const noexcept
    {
        T num  = value_.num * rhs.value_.deno + rhs.value_.num * value_.deno;
        T deno = value_.deno * rhs.value_.deno;
        return Rational{num, deno};
    }
    Rational operator-(const Rational& rhs) const noexcept
    {
        T num  = value_.num * rhs.value_.deno - rhs.value_.num * value_.deno;
        T deno = value_.deno * rhs.value_.deno;
        return Rational{num, deno};
    }
    Rational operator*(const Rational& rhs) const noexcept
    {
        return Rational{value_.num * rhs.value_.num, value_.deno * rhs.value_.deno};
    }
    Rational operator/(const Rational& rhs) const noexcept
    {
        return Rational(value_.num * rhs.value_.deno, value_.deno * rhs.value_.num);
    }
    /// @brief 複合代入演算子(四則演算)の定義
    Rational& operator+=(Rational const& rhs)
    {
        *this = *this + rhs;
        return *this;
    }
    Rational& operator-=(Rational const& rhs)
    {
        *this = *this - rhs;
        return *this;
    }
    Rational& operator*=(Rational const& rhs)
    {
        *this = *this * rhs;
        return *this;
    }
    Rational& operator/=(Rational const& rhs)
    {
        *this = *this / rhs;
        return *this;
    }
    /// @brief 単項演算子の定義
    constexpr Rational operator+() const noexcept { return *this; }
    constexpr Rational operator-() const noexcept { return Rational{-value_.num, value_.deno}; }
    /// @brief 比較演算子の定義
#if __cplusplus == 202002L
    friend bool operator==(Rational const& lhs, Rational const& rhs) noexcept = default;
    friend auto operator<=>(Rational const& lhs, Rational const& rhs) noexcept
    {
        return (lhs.value_.num * rhs.value_.deno) <=> (rhs.value_.num * lhs.value_.deno);
    }
#else
    friend bool operator==(Rational const& lhs, Rational const& rhs) noexcept
    {
        return (lhs.value_.num * rhs.value_.deno) == (rhs.value_.num * lhs.value_.deno);
    }
    friend bool operator!=(Rational const& lhs, Rational const& rhs) noexcept
    {
        return !(lhs == rhs);
    }
    friend bool operator<(Rational const& lhs, Rational const& rhs) noexcept
    {
        return (lhs.value_.num * rhs.value_.deno) < (rhs.value_.num * lhs.value_.deno);
    }
    friend bool operator<=(Rational const& lhs, Rational const& rhs) noexcept
    {
        return !(rhs < lhs);
    }
    friend bool operator>(Rational const& lhs, Rational const& rhs) noexcept { return rhs < lhs; }
    friend bool operator>=(Rational const& lhs, Rational const& rhs) noexcept
    {
        return !(lhs < rhs);
    }
#endif
    /// @brief put-to演算子の定義
    friend std::ostream& operator<<(std::ostream& os, const Rational& rhs)
    {
        return (rhs.value_.deno == 1) ? os << rhs.value_.num
                                      : os << rhs.value_.num << "/" << rhs.value_.deno;
    }
    /// @brief doubleへの変換演算子
    /// @brief doubleで表現可能な場合のみ利用可能
    template <typename U = T>
    explicit operator double() const noexcept
#if __cplusplus == 202002L
        requires std::is_convertible_v<U, double>
#endif
    {
        return static_cast<double>(value_.num) / static_cast<double>(value_.deno);
    }

private:
    struct rational_t {
        T num;
        T deno;
#if __cplusplus == 202002L
        friend bool operator==(rational_t const& lhs, rational_t const& rhs) noexcept = default;
#else
        friend bool operator==(rational_t const& lhs, rational_t const& rhs) noexcept
        {
            return lhs.num == rhs.num && lhs.deno == rhs.deno;
        }
#endif
    };
    /// @brief num/denoを約分して、rational_tで返す
    static rational_t reduce(T num, T deno) noexcept
    {
        T const gcd = std::gcd(num, deno);
        num /= gcd;
        deno /= gcd;
        if (deno < 0) {  // 分母を正に保つ
            return {-num, -deno};
        }
        else {
            return {num, deno};
        }
    }

private:
    rational_t value_;
};
}  // namespace Nstd
// @@@ sample end
// @@@ sample begin 0:1

namespace Nstd {
/// @brief Rational<int32_t>をユーザ定義リテラルとして扱うためのオペレータ
///        _rn: rational number
///  @param N/Mの形式の文字列リテラル
Rational<int32_t> operator"" _rn(char const* str, size_t)
{
    std::string_view input{str};
    size_t           sep = input.find('/');

    if (sep == std::string::npos) {
        throw std::invalid_argument("invalid formt for user-defined rational number");
    }

    int32_t num  = std::stoi(str);
    int32_t deno = std::stoi(input.substr(sep + 1).data());

    return {num, deno};
}
}  // namespace Nstd
// @@@ sample end
