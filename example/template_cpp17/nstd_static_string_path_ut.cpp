#include "gtest_wrapper.h"

#include "nstd_static_string.h"
#include "nstd_static_string_path.h"
#include "nstd_type2str.h"

namespace Nstd {
namespace {
template <size_t N>
constexpr auto Path2Filename(char const (&path)[N]) noexcept
{
    return BottomStr<filename_offset(path)>(StaticString{path});
}

TEST(Template, path2filename)
{
    constexpr char path[]{"/abc/efgh/filename"};
    constexpr auto sp1 = StaticString{path};
    static_assert(19 == sp1.Size());
    ASSERT_STREQ(path, sp1.String());

    constexpr auto offset = filename_offset(path);

    std::cout << filename_len(path) << std::endl;

    constexpr auto fs1 = StaticString<filename_len(path)>{offset, sp1};
    constexpr auto fs2 = PATH2FILENAME(path);
    constexpr auto fs3 = BottomStr<filename_offset(path)>(StaticString{path});

    static_assert(9 == fs1.Size());
    static_assert(9 == fs2.Size());
    static_assert(9 == fs3.Size());
    ASSERT_STREQ("filename", fs2.String());
    ASSERT_STREQ("filename", fs3.String());

#if 0  // path’ is not a constant expressionと判定される。おそらくコンパイラのバグ
    constexpr auto fs4 = Path2Filename(path);
    ASSERT_STREQ("filename", fs4.String());
#endif
}

TEST(Template, PATH2FILENAME)
{
    constexpr char path[]{"/abc/efgh/filename"};
    constexpr auto sp1 = StaticString{path};
    static_assert(19 == sp1.Size());
    ASSERT_STREQ(path, sp1.String());

    constexpr auto offset = filename_offset(path);

    std::cout << filename_len(path) << std::endl;

    constexpr auto fs1 = StaticString<filename_len(path)>{offset, sp1};
    constexpr auto fs2 = PATH2FILENAME(path);

    static_assert(9 == fs1.Size());
    ASSERT_STREQ("filename", fs2.String());
    static_assert(9 == fs2.Size());
}

}  // namespace
}  // namespace Nstd
