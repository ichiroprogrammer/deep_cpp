#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "gtest_wrapper.h"

#include "visitor_crtp.h"

namespace {
std::string find_files(std::string const& dir)
{
    namespace fs = std::filesystem;  // OK 長い名前を短く

    auto files = std::vector<std::string>{};
    auto top   = fs::path{dir};

    std::for_each(fs::recursive_directory_iterator{top}, fs::recursive_directory_iterator{},
                  [&](fs::path const& p) { files.emplace_back(p.generic_string()); });

    std::sort(files.begin(), files.end());

    auto ret = std::string{};

    for (auto f : files) {
        if (ret.size() != 0) {
            ret += "," + f;
        }
        else {
            ret = f;
        }
    }

    return ret;
}
}  // namespace

namespace CRTP {

void PathnamePrinter1::Visit(File const& file) { std::cout << file.Pathname(); }
void PathnamePrinter1::Visit(Dir const& dir) { std::cout << dir.Pathname() + "/"; }
void PathnamePrinter1::Visit(OtherEntity const& other) { std::cout << other.Pathname() + "(o1)"; }

void PathnamePrinter2::Visit(File const& file) { std::cout << file.Pathname(); }
void PathnamePrinter2::Visit(Dir const& dir) { std::cout << find_files(dir.Pathname()); }
void PathnamePrinter2::Visit(OtherEntity const& other) { std::cout << other.Pathname() + "(o2)"; }

class Printer {
public:
    static void PrintPathname1(FileEntity const& file_entity)
    {
        auto visitor = PathnamePrinter1{};

        file_entity.Accept(visitor);
    }

    static void PrintPathname2(FileEntity const& file_entity)
    {
        auto visitor = PathnamePrinter2{};

        file_entity.Accept(visitor);
    }
};

void TestablePathnamePrinter1::Visit(File const& file) { ostream_ << file.Pathname(); }
void TestablePathnamePrinter1::Visit(Dir const& dir) { ostream_ << dir.Pathname() + "/"; }
void TestablePathnamePrinter1::Visit(OtherEntity const& other) { ostream_ << other.Pathname() + "(o1)"; }

void TestablePathnamePrinter2::Visit(File const& file) { ostream_ << file.Pathname(); }

void TestablePathnamePrinter2::Visit(Dir const& dir) { ostream_ << find_files(dir.Pathname()); }

void TestablePathnamePrinter2::Visit(OtherEntity const& other) { ostream_ << other.Pathname() + "(o2)"; }
}  // namespace CRTP
namespace CRTP {
// @@@ sample begin 0:0

template <typename T>
class Base {
    // @@@ ignore begin
    // @@@ ignore end
};

class Derived : public Base<Derived> {
    // @@@ ignore begin
    // @@@ ignore end
};
// @@@ sample end

namespace {

TEST(CRTP, testable_visitor)
{
    auto ostring = std::ostringstream{};

    // 出力をキャプチャするため、std::coutに代えてostringを使う
    auto visitor1 = TestablePathnamePrinter1{ostring};
    auto visitor2 = TestablePathnamePrinter2{ostring};

    auto file = File{"visitor.cpp"};
    {
        file.Accept(visitor1);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("visitor.cpp", ret);
        ostring = std::ostringstream{};
    }
    {
        file.Accept(visitor2);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("visitor.cpp", ret);
        ostring = std::ostringstream{};
    }

    auto dir = Dir{"find_files_ut_dir/dir0"};
    {
        dir.Accept(visitor1);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("find_files_ut_dir/dir0/", ret);
        ostring = std::ostringstream{};
    }
    {
        dir.Accept(visitor2);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ("find_files_ut_dir/dir0/file2,find_files_ut_dir/dir0/gile3", ret);
        ostring = std::ostringstream{};
    }

    auto other = OtherEntity{"."};
    {
        other.Accept(visitor1);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ(".(o1)", ret);
        ostring = std::ostringstream{};
    }
    {
        other.Accept(visitor2);
        auto ret = std::string{ostring.str()};
        ASSERT_EQ(".(o2)", ret);
        ostring = std::ostringstream{};
    }
}
}  // namespace
}  // namespace CRTP
