#include <coroutine>
#include <iostream>
#include <memory>
#include <sstream>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace use_coroutine_pipeline {

// @@@ sample begin 0:0

template <typename T>
class Generator {
public:
    Generator(Generator&& other) noexcept : coro{std::move(other.coro)} { other.coro = nullptr; }

    Generator& operator=(Generator&& other) noexcept
    {
        if (this != &other) {
            coro       = other.coro;
            other.coro = nullptr;
        }
        return *this;
    }

    /// @struct promise_type
    /// @brief コルーチンのライフサイクルを管理する構造体
    struct promise_type {
        T current_value;

        /// @fn Generator get_return_object()
        /// @brief コルーチンから Generator 型のオブジェクトを返す関数
        /// @return Generatorオブジェクト
        Generator get_return_object()
        {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        /// @fn std::suspend_always initial_suspend()
        /// @brief コルーチンの最初のサスペンドポイント
        /// @return 常にサスペンドするオブジェクトを返す
        std::suspend_always initial_suspend() { return {}; }

        /// @fn std::suspend_always final_suspend() noexcept
        /// @brief コルーチンの最後のサスペンドポイント
        /// @return 常にサスペンドするオブジェクトを返す
        std::suspend_always final_suspend() noexcept { return {}; }

        /// @fn std::suspend_always yield_value(T value)
        /// @brief コルーチンで値を生成するためのサスペンドポイント
        /// @param value 生成された値
        /// @return 常にサスペンドするオブジェクトを返す
        std::suspend_always yield_value(T value)
        {
            current_value = value;
            return {};
        }

        /// @fn void unhandled_exception()
        /// @brief コルーチン内で例外が発生した場合に呼び出される
        void unhandled_exception() { std::exit(1); }

        /// @fn void return_void()
        /// @brief コルーチンの終了時に呼び出される
        void return_void() {}
    };

    /// @fn bool move_next()
    /// @brief コルーチンを再開し、次の値を生成する
    /// @return 次の値が生成された場合は true、終了した場合は false
    bool move_next()
    {
        if (coro && !coro.done()) {
            coro.resume();
            return !coro.done();
        }
        return false;
    }

    /// @fn T current_value() const
    /// @brief 現在の値を取得する
    /// @return 現在の値
    T current_value() const { return coro.promise().current_value; }

    /// @brief Generator のコンストラクタ
    /// @param h コルーチンハンドル
    Generator(std::coroutine_handle<promise_type> h) : coro(h) {}

    /// @brief Generator のデストラクタ
    /// @details コルーチンハンドルが有効であれば破棄する
    ~Generator()
    {
        if (coro) coro.destroy();
    }

private:
    std::coroutine_handle<promise_type> coro;
};

/// @fn Generator<int> filter_even(Generator<int> input)
/// @brief 偶数のみをフィルタリングする
/// @param input フィルタ対象の Generator
/// @return フィルタ後の Generator
Generator<int> filter_even(Generator<int> input)
{
    while (input.move_next()) {
        if (input.current_value() % 2 == 0) {
            co_yield input.current_value();
        }
    }
}

/// @fn Generator<int> double_values(Generator<int> input)
/// @brief 値を2倍に変換する
/// @param input 変換対象の Generator
/// @return 変換後の Generator
Generator<int> double_values(Generator<int> input)
{
    while (input.move_next()) {
        co_yield input.current_value() * 2;
    }
}

/// @fn Generator<int> generate_numbers(int start, int end)
/// @brief 数値の範囲を生成する
/// @param start 開始値
/// @param end 終了値
/// @return 範囲内の数値を生成する Generator
Generator<int> generate_numbers(int start, int end)
{
    for (int i = start; i <= end; ++i) {
        co_yield i;
    }
}
// @@@ sample end

TEST(ExpTerm, use_co_yield)
{
    // @@@ sample begin 0:1

    // 数値を生成し、それをパイプライン処理に通す
    auto numbers         = generate_numbers(1, 10);
    auto even_numbers    = filter_even(std::move(numbers));
    auto doubled_numbers = double_values(std::move(even_numbers));

    // 結果を検証するために期待される値の配列を準備
    std::vector<int> expected_values = {4, 8, 12, 16, 20};

    // 生成された値を順に取得し、期待される値と比較
    size_t index = 0;
    while (doubled_numbers.move_next()) {
        ASSERT_LT(index, expected_values.size());  // インデックスが範囲内か確認
        EXPECT_EQ(doubled_numbers.current_value(), expected_values[index]);
        ++index;
    }

    // 最終的にすべての期待される値が生成されたことを確認
    EXPECT_EQ(index, expected_values.size());
    // @@@ sample end
}
}  // namespace use_coroutine_pipeline

namespace no_use_coroutine_pipeline {
// @@@ sample begin 1:0

/// @class Generator
/// @brief コルーチンを使わずにデータを逐次的に提供するジェネレータークラス
template <typename T>
class Generator {
public:
    /// @brief コンストラクタ
    /// @param data 生成対象のデータ
    Generator(std::vector<T>&& data) : data_(std::move(data)), current_index_(0) {}

    /// @fn bool move_next()
    /// @brief 次の値があるかを確認し、次の値に進む
    /// @return 次の値が存在する場合は true、存在しない場合は false
    bool move_next()
    {
        if (current_index_ < data_.size()) {
            ++current_index_;
            return true;
        }
        return false;
    }

    /// @fn T current_value() const
    /// @brief 現在の値を取得する
    /// @return 現在の値
    T current_value() const
    {
        if (current_index_ > 0 && current_index_ <= data_.size()) {
            return data_[current_index_ - 1];
        }
        throw std::out_of_range("Invalid current value access");
    }

private:
    std::vector<T> data_;           ///< データを保持
    size_t         current_index_;  ///< 現在のインデックス
};

/// @fn Generator<int> filter_even(const Generator<int>& input)
/// @brief 偶数のみをフィルタリングする
/// @param input フィルタ対象の Generator
/// @return フィルタ後の Generator
Generator<int> filter_even(const Generator<int>& input)
{
    std::vector<int> filtered;
    auto             gen = input;

    while (gen.move_next()) {
        if (gen.current_value() % 2 == 0) {
            filtered.push_back(gen.current_value());
        }
    }
    return Generator<int>(std::move(filtered));
}

/// @fn Generator<int> double_values(const Generator<int>& input)
/// @brief 値を2倍に変換する
/// @param input 変換対象の Generator
/// @return 変換後の Generator
Generator<int> double_values(const Generator<int>& input)
{
    std::vector<int> doubled;
    auto             gen = input;

    while (gen.move_next()) {
        doubled.push_back(gen.current_value() * 2);
    }
    return Generator<int>(std::move(doubled));
}

/// @fn Generator<int> generate_numbers(int start, int end)
/// @brief 数値の範囲を生成する
/// @param start 開始値
/// @param end 終了値
/// @return 範囲内の数値を生成する Generator
Generator<int> generate_numbers(int start, int end)
{
    std::vector<int> numbers;
    for (int i = start; i <= end; ++i) {
        numbers.push_back(i);
    }
    return Generator<int>(std::move(numbers));
}
// @@@ sample end

TEST(ExpTerm, no_use_co_yield)
{
    // @@@ sample begin 1:1

    // 数値を生成し、それをパイプライン処理に通す
    auto numbers         = generate_numbers(1, 10);
    auto even_numbers    = filter_even(std::move(numbers));
    auto doubled_numbers = double_values(std::move(even_numbers));

    // 結果を検証するために期待される値の配列を準備
    std::vector<int> expected_values = {4, 8, 12, 16, 20};

    // 生成された値を順に取得し、期待される値と比較
    size_t index = 0;
    while (doubled_numbers.move_next()) {
        ASSERT_LT(index, expected_values.size());  // インデックスが範囲内か確認
        EXPECT_EQ(doubled_numbers.current_value(), expected_values[index]);
        ++index;
    }

    // 最終的にすべての期待される値が生成されたことを確認
    EXPECT_EQ(index, expected_values.size());
    // @@@ sample end
}
}  // namespace no_use_coroutine_pipeline
