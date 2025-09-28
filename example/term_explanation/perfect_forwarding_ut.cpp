#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace {

// @@@ sample begin 0:0

class Widget {
public:
    explicit Widget(std::string const& name) : name_{name} {}  // lvalueによるコンストラクタ
    explicit Widget(std::string&& name) : name_{std::move(name)} {}  // rvalueによるコンストラクタ
    std::string const& GetName() const { return name_; }

private:
    std::string name_;  // コンストラクタの引数をcopy/move構築
};

template <typename T>
Widget make_Widget(T&& str)
{                                         // strはforwardingリファレンス
    return Widget(std::forward<T>(str));  // perfect forwarding
}
// @@@ sample end

TEST(PerfectForwarding, forwarding)
{
    // @@@ sample begin 0:1

    std::string       str{"lvalue ref"};
    std::string const cstr{"lvalue const ref"};

    Widget w0 = make_Widget(str);  // make_Widget -> Widget(std::string const&)
    ASSERT_EQ(w0.GetName(), str);

    Widget w1 = make_Widget(cstr);  // make_Widget -> Widget(std::string const&)
    ASSERT_EQ(w1.GetName(), cstr);

    Widget w2 = make_Widget(std::string{"rvalue ref"});  // make_Widget -> Widget(std::string &&)
    ASSERT_EQ(w2.GetName(), "rvalue ref");

    Widget w3 = make_Widget(std::move(str));  // make_Widget -> Widget(std::string &&)
    ASSERT_EQ(w3.GetName(), "lvalue ref");    // strはムーブされたのでアクセス不可
    // @@@ sample end
}
}  // namespace
