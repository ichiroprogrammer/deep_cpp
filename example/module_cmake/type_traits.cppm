// @@@ sample begin 0:0

export module type_traits;  // モジュール宣言

namespace type_traits {  // 通常の名前空間。モジュール名と同じにして良い
namespace Inner_ {       // 内部実装であるためexportしない

// T == void
template <typename T>
constexpr auto is_void_sfinae_f_detector(void const* v, T const* t) noexcept
    -> decltype(t = v, bool{})  // T != voidの場合、t = vはill-formed
                                // T == voidの場合、well-formedでbool型生成
{
    return true;
}

// T != void
template <typename T>
constexpr auto is_void_sfinae_f_detector(void const*, T const*) noexcept
    -> decltype(sizeof(T), bool{})  // T != voidの場合、well-formedでbool型生成
                                    // T == voidの場合、sizeof(T)はill-formed
{
    return false;
}
}  // namespace Inner_

export
{  // 纏めてexport
    template <typename T>
    constexpr bool is_void() noexcept
    {
        return Inner_::is_void_sfinae_f_detector(nullptr, static_cast<T*>(nullptr));
    }
    template <typename T>
    bool is_void_v = is_void<T>();
}
}  // namespace type_traits
// @@@ sample end
