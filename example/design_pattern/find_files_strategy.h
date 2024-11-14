#include <algorithm>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

// @@@ sample begin 0:0

/// @typedef find_condition
/// @brief find_files_recursively仮引数conditionの型(関数オブジェクトの型)
using find_condition = std::function<bool(std::filesystem::path const&)>;

// Strategyパターン
/// @fn std::vector<std::string> find_files_recursively(std::string const& path,
///                                                     find_condition     condition);
/// @brief 条件にマッチしたファイルをリカーシブに探索して返す
/// @param path      リカーシブにディレクトリを辿るための起点となるパス
/// @param condition 探索するファイルの条件
/// @return 条件にマッチしたファイルをstd::vector<std::string>で返す
extern std::vector<std::string> find_files_recursively(std::string const& path,
                                                       find_condition     condition);
// @@@ sample end
// @@@ sample begin 1:0

#if __cplusplus >= 202002L  // c++20

// ファンクタがboolを返し、std::filesystem::path const&を引数に取るかを確認するコンセプト
// clang-format off
namespace Inner_ {
template <typename F>
concept find_condition = requires(F f, std::filesystem::path const& p)
{
    { f(p) } -> std::same_as<bool>;
};
// clang-format on
}  // namespace Inner_

template <Inner_::find_condition F>
auto find_files_recursively2(std::string const& path, F condition)
    -> std::enable_if_t<std::is_invocable_r_v<bool, F, std::filesystem::path const&>,
                        std::vector<std::string>>

#else  // c++17

template <typename F>  // Fはファンクタ
auto find_files_recursively2(std::string const& path, F&& condition) -> std::vector<std::string>
#endif
{
    namespace fs = std::filesystem;

    auto files = std::vector<fs::path>{};

    // recursive_directory_iteratorでディレクトリ内のファイルを再帰的に取得
    std::copy(fs::recursive_directory_iterator{path}, fs::recursive_directory_iterator{},
              std::back_inserter(files));

    std::sort(files.begin(), files.end());  // ファイルリストをソート

    auto ret = std::vector<std::string>{};

    std::for_each(files.cbegin(), files.cend(), [&](fs::path const& p) {
        if (condition(p)) {  // 条件を満たすファイルをretに追加
            ret.emplace_back(p.generic_string());
        }
    });

    return ret;
}
// @@@ sample end
