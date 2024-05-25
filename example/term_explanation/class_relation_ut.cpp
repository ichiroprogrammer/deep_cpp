#include <memory>
#include <stdexcept>
#include <vector>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

namespace is_a_0 {

// @@@ sample begin 0:0

class bird {
public:
    //  事前条件: altitude  > 0 でなければならない
    //  事後条件: 呼び出しが成功した場合、is_flyingがtrueを返すことである
    virtual void fly(int altitude)
    {
        if (not(altitude > 0)) {  // 高度(altitude)は0より大きくなければ、飛べない
            throw std::invalid_argument{"altitude error"};
        }
        altitude_ = altitude;
    }

    bool is_flying() const noexcept
    {
        return altitude_ != 0;  // 高度が0でなければ、飛んでいると判断
    }

    virtual ~bird() = default;

private:
    int altitude_ = 0;
};

class kyukancho : public bird {
public:
    void speak()
    {
        // しゃべるため処理
    }

    // このクラスにget_nameを追加した理由はこの後を読めばわかる
    virtual std::string get_name() const  // その個体の名前を返す
    {
        return "no name";
    }
};
// @@@ sample end
// @@@ sample begin 0:1

class penguin : public bird {
public:
    void fly(int altitude) override
    {
        if (altitude != 0) {
            throw std::invalid_argument{"altitude error"};
        }
    }
};
// @@@ sample end

TEST(ClassRelation, is_a)
{
    // @@@ sample begin 0:2

    // ...

    auto let_it_fly = [](bird& b, int altitude) {
        try {
            b.fly(altitude);
        }
        catch (std::exception const&) {
            return 0;  // エクセプションが発生した
        }

        return b.is_flying() ? 2 : 1;  // is_flyingがfalseなら1を返す
    };

    bird    b;
    penguin p;
    ASSERT_EQ(let_it_fly(p, 0), 1);  // パスする
    // birdからpenguinへの派生がリスコフ置換の原則を満たすのであれば、
    // 上記のテストのpをbで置き換えたテストがパスしなければならないが、
    // 実際には逆に下記テストがパスしてしまう
    ASSERT_NE(let_it_fly(b, 0), 1);
    // このことからpenguinへの派生はリスコフ置換の原則を満たさない

    // @@@ sample end
}

// @@@ sample begin 1:0

class q_chan : public kyukancho {
public:
    std::string get_name() const override { return "キューちゃん"; }
};
// @@@ sample end

TEST(ClassRelation, is_a2)
{
    penguin   pe;
    kyukancho kyu;
    q_chan    q;

    ASSERT_EQ("no name", kyu.get_name());
    ASSERT_EQ("no name", kyu.get_name());
    ASSERT_EQ("キューちゃん", q.get_name());
}
}  // namespace is_a_0

namespace is_a_1 {

// @@@ sample begin 2:0

class kyukancho {
public:
    kyukancho(std::string name) : name_{std::move(name)} {}

    std::string const& get_name() const  // 名称をメンバ変数で保持するため、virtualである必要はない
    {
        return name_;
    }

    virtual ~kyukancho() = default;

private:
    std::string const name_;  // 名称の保持
};
// @@@ sample end

TEST(ClassRelation, is_a3)
{
    // @@@ sample begin 2:1

    // ...

    kyukancho q{"キューちゃん"};

    ASSERT_EQ("キューちゃん", q.get_name());
    // @@@ sample end
}
}  // namespace is_a_1
namespace ha_a {
// @@@ sample begin 3:0

class Engine {
public:
    void start() {}  // エンジンを始動するための処理
    void stop() {}   // エンジンを停止するための処理

private:
    // ...
};

class Car {
public:
    Car() : engine_{} {}
    void start() { engine_.start(); }
    void stop() { engine_.stop(); }

private:
    Engine engine_;  // Car は Engine を持っている（has-a）
};
// @@@ sample end

// 使用例
TEST(ClassRelation, ha_a)
{
    Car myCar;
    myCar.start();
    myCar.stop();
}
}  // namespace ha_a

namespace is_implemented_in_terms_of_0 {

// is-implemented-in-terms-ofにprivate継承を使用した例

// @@@ sample begin 4:0

class MyString : std::string {
public:
    using std::string::string;
    using std::string::operator[];
    using std::string::c_str;
    using std::string::clear;
    using std::string::size;
};
// @@@ sample end

TEST(ClassRelation, is_implemented_in_terms_of_0)
{
    // @@@ sample begin 4:1

    // ...
    MyString str{"str"};

    ASSERT_EQ(str[0], 's');
    ASSERT_STREQ(str.c_str(), "str");

    str.clear();
    ASSERT_EQ(str.size(), 0);
    // @@@ sample end
}
}  // namespace is_implemented_in_terms_of_0

// @@@ sample begin 5:0

namespace is_implemented_in_terms_of_1 {
class MyString {
public:
    // コンストラクタ
    MyString() = default;
    MyString(const std::string& str) : str_(str) {}
    MyString(const char* cstr) : str_(cstr) {}

    // 文字列へのアクセス
    const char* c_str() const { return str_.c_str(); }

    using reference = std::string::reference;
    using size_type = std::string::size_type;

    reference operator[](size_type pos) { return str_[pos]; }

    // その他のメソッドも必要に応じて追加する
    // 以下は例
    std::size_t size() const { return str_.size(); }

    void clear() { str_.clear(); }

    MyString& operator+=(const MyString& rhs)
    {
        str_ += rhs.str_;
        return *this;
    }

private:
    std::string str_;
};
// @@@ sample end

TEST(ClassRelation, is_implemented_in_terms_of_0)
{
    // @@@ sample begin 5:1

    // ...
    MyString str{"str"};

    ASSERT_EQ(str[0], 's');
    ASSERT_STREQ(str.c_str(), "str");

    str.clear();
    ASSERT_EQ(str.size(), 0);
    // @@@ sample end
    // @@@ sample end
}
}  // namespace is_implemented_in_terms_of_1
namespace is_implemented_in_terms_of_2 {

// @@@ sample begin 6:0

class MyString : public std::string {  // 手段1
};
// @@@ sample end

TEST(ClassRelation, is_implemented_in_terms_of_2)
{
    // @@@ sample begin 6:1

    // ...
    std::string* m_str = new MyString{"str"};

    // このようなpublic継承を行う場合、基底クラスのデストラクタは非virtualであるため、
    // 以下のコードではｈmy_stringのデストラクタは呼び出されない。
    // この問題はリソースリークを発生させる場合がある。
    delete m_str;
    // @@@ sample end
}
}  // namespace is_implemented_in_terms_of_2
namespace is_implemented_in_terms_of_3 {

// @@@ sample begin 7:0

class MyString : public std::string {};
// @@@ sample end

TEST(ClassRelation, is_implemented_in_terms_of_3)
{
    // @@@ sample begin 7:1

    // ...
    MyString str{"str"};

    ASSERT_EQ(str[0], 's');
    ASSERT_STREQ(str.c_str(), "str");

    str.clear();
    ASSERT_EQ(str.size(), 0);
    // @@@ sample end
}
}  // namespace is_implemented_in_terms_of_3
