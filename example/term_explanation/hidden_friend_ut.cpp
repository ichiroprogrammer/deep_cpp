#include <sstream>

#include "gtest_wrapper.h"

#include "suppress_warning.h"

// @@@ sample begin 0:0

namespace NS {
class Person {
public:
    Person(std::string name, uint32_t age) : name_{std::move(name)}, age_{age} {}

    // hidden-friend関数
    friend std::ostream& operator<<(std::ostream& os, const Person& person)
    {
        os << "Name:" << person.name_ << ", Age:" << person.age_;
        return os;
    }

private:
    std::string const name_;
    uint32_t const    age_;
};
}  // namespace NS
// @@@ sample end

namespace {
TEST(ExpTerm, hidden_friend)
{
    // @@@ sample begin 0:1

    NS::Person         alice("Alice", 30);
    std::ostringstream oss;

    oss << alice;  // フレンド関数を呼び出す(ADLによって見つかる)
    ASSERT_EQ("Name:Alice, Age:30", oss.str());

    // 以下はエラー（operator<<がNS名前空間スコープで見えない）
    // NS::Person::operator<<(oss, alice);
    // 上記は以下のようなコンパイルエラーになる
    //  error: ‘operator<<’ is not a member of ‘NS::Person’
    // @@@ sample end
}
}  // namespace
