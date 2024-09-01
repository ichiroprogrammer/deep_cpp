
#pragma once
#include "nstd_static_string.h"

namespace Nstd {
template <size_t N>
constexpr size_t filename_len(const char (&path)[N], size_t index = N - 1) noexcept
{
    if (index == 0 || path[index - 1] == '/') {
        return N - index;
    }
    return filename_len(path, index - 1);
}

template <size_t N>
constexpr size_t filename_offset(const char (&path)[N]) noexcept
{
    return N - filename_len(path);
}

// 関数テンプレートにすべきだが、下の関数テンプレートはpathがconstexprと評価されない
#define PATH2FILENAME(path__) BottomStr<filename_offset(path__)>(StaticString{path__})

}  // namespace Nstd
